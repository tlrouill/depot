#include "controller.h"
#include "position.h"
#include "motor.h"

#define CTRL_T  (CTRL_T_MS / 1000.0f)       // période en secondes

static volatile uint8_t enabled = 0;
static volatile int32_t target  = 0;
static float   integral = 0.0f;             // T * somme(e)
static int32_t output   = 0;

void controller_enable(void)
{
    integral = 0.0f;
    enabled  = 1;
}

void controller_set_target(int32_t r)
{
    if (r >  100) r =  100;
    if (r < -100) r = -100;
    target = r;
}

void controller_update(void)
{
    if (!enabled) return;

    int32_t e = target - position_read();

    // Arrivé : moteur coupé, intégrale vidée (sinon le moteur oscille
    // autour de la cible à cause du frottement)
    if (e >= -CTRL_DEADZONE && e <= CTRL_DEADZONE) {
        integral = 0.0f;
        output   = 0;
        motor_set(0);
        return;
    }

    // Loi PI
    float u = CTRL_KP * (float)e + CTRL_KI * (integral + CTRL_T * (float)e);

    // Saturation ; on n'intègre que si la commande n'est pas saturée
    // (anti-windup : pas d'intégrale qui explose quand le chariot est bloqué)
    if      (u >  CTRL_U_MAX) u =  CTRL_U_MAX;
    else if (u < -CTRL_U_MAX) u = -CTRL_U_MAX;
    else    integral += CTRL_T * (float)e;

    // Puissance minimale : en dessous, le frottement empêche tout mouvement
    if (u > 0 && u <  CTRL_U_MIN) u =  CTRL_U_MIN;
    if (u < 0 && u > -CTRL_U_MIN) u = -CTRL_U_MIN;

    output = (int32_t)u;
    motor_set(output);
}

int32_t controller_get_target(void) { return target; }
int32_t controller_get_output(void) { return output; }