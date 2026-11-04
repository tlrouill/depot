#include "controller.h"
#include "position.h"
#include "motor.h"

#define CTRL_T  (CTRL_T_MS / 1000.0f)    // période en secondes

static volatile uint8_t enabled  = 0;
static volatile int32_t target   = 0;    // écrite par main, lue dans l'interruption
static float            integral = 0.0f; // T * somme(e)
static int32_t          output   = 0;

void controller_enable(void)
{
    integral = 0.0f;
    output   = 0;
    enabled  = 1;
}

void controller_disable(void)
{
    enabled = 0;
    output  = 0;
    motor_stop();
}

void controller_set_target(int32_t r)
{
    if (r >  100) r =  100;
    if (r < -100) r = -100;
    target = r;                          // écriture 32 bits : atomique sur Cortex-M3
}

void controller_update(void)
{
    if (!enabled || !position_is_calibrated()) return;

    int32_t e = target - position_read();

    // 1. Assez proche de la consigne : on coupe le moteur et on vide
    //    l'intégrale. (Sinon le moteur "tremble" autour de la cible, et
    //    l'intégrale restante freinerait le déplacement suivant.)
    if (e >= -CTRL_DEADZONE && e <= CTRL_DEADZONE) {
        integral = 0.0f;
        output = 0;
        motor_set(0);
        return;
    }

    // 2. Partie intégrale : T * somme(e), bornée à ±CTRL_I_MAX (anti-windup).
    //    Sans borne, si le chariot est bloqué, l'intégrale grandirait
    //    sans fin et le moteur partirait violemment une fois libéré.
    float p_part = CTRL_KP * (float)e;
    integral += CTRL_T * (float)e;
    float i_part = CTRL_KI * integral;
    if (i_part >  CTRL_I_MAX) { i_part =  CTRL_I_MAX; integral =  CTRL_I_MAX / CTRL_KI; }
    if (i_part < -CTRL_I_MAX) { i_part = -CTRL_I_MAX; integral = -CTRL_I_MAX / CTRL_KI; }

    // 3. Loi PI
    float u = p_part + i_part;

    // 4. Bornes de la commande
    if (u >  CTRL_U_MAX) u =  CTRL_U_MAX;
    if (u < -CTRL_U_MAX) u = -CTRL_U_MAX;

    // 5. Compensation du frottement : en dessous de CTRL_U_MIN le moteur
    //    ne bouge pas du tout, donc on donne au moins CTRL_U_MIN.
    if (u > 0 && u <  CTRL_U_MIN) u =  CTRL_U_MIN;
    if (u < 0 && u > -CTRL_U_MIN) u = -CTRL_U_MIN;

    output = (int32_t)u;
    motor_set(output);
}

int32_t controller_get_target(void) { return target; }
int32_t controller_get_output(void) { return output; }