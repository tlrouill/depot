#include "controller.h"
#include "position.h"
#include "motor.h"

#define CTRL_T  (CTRL_T_MS / 1000.0f)       // période en secondes

static volatile uint8_t enabled = 0;
static volatile int32_t target  = 0;
static float   integral = 0.0f;             // T * somme(e)
static int32_t output   = 0;
static int32_t last_target  = 0;            // consigne au tick précédent
static uint8_t moving_ticks = 0;            // > 0 : la consigne a bougé il y a moins de 100 ms

void controller_enable(void)
{
    integral = 0.0f;
    enabled  = 1;
}

void controller_set_target(int32_t r)
{
    target = r;                             // en ticks (bornée par main.c)
}

void controller_update(void) // written with the help of AI 
{
    if (!enabled) return;

    // Erreur en ticks (pleine résolution), convertie en "unités" -100..100
    // SANS arrondi : Kp, Ki et la zone morte gardent leurs valeurs habituelles
    float e = (float)(target - position_read()) / position_unit();

    // La consigne bouge-t-elle ? (info gardée 100 ms = 10 ticks)
    if (target != last_target) moving_ticks = 10;
    else if (moving_ticks > 0) moving_ticks--;
    last_target = target;

    // Arrivé ET consigne immobile : moteur coupé, intégrale vidée.
    // Pendant un déplacement, on ne coupe jamais le moteur.
    if (moving_ticks == 0 && e >= -CTRL_DEADZONE && e <= CTRL_DEADZONE) {
        integral = 0.0f;
        output   = 0;
        motor_set(0);
        return;
    }

    // Loi PI
    float u = CTRL_KP * e + CTRL_KI * (integral + CTRL_T * e);

    // Saturation ; on n'intègre que si la commande n'est pas saturée
    // (anti-windup : pas d'intégrale qui explose quand le chariot est bloqué)
    if      (u >  CTRL_U_MAX) u =  CTRL_U_MAX;
    else if (u < -CTRL_U_MAX) u = -CTRL_U_MAX;
    else   integral += CTRL_T * e;

    // Puissance minimale : en dessous, le frottement empêche tout mouvement
    if (u > 0 && u <  CTRL_U_MIN) u =  CTRL_U_MIN;
    if (u < 0 && u > -CTRL_U_MIN) u = -CTRL_U_MIN;

    output = (int32_t)u;
    motor_set(output);
}

int32_t controller_get_target(void) { return target; }
int32_t controller_get_output(void) { return output; }