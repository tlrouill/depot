#include "sam.h"
#include "motor.h"
#include "pwm.h"

#define MOTOR_DIR_PIN  PIO_PC23

static int32_t current_u = 0;

// Convertit une puissance 0..100 % en temps haut (µs) pour le driver PWM
static uint32_t duty_to_high_us(uint32_t duty)
{
    uint32_t high = duty * MOTOR_PERIOD_US / 100;
#if MOTOR_PWM_INVERT
    high = MOTOR_PERIOD_US - high;
#endif
    return high;
}

static void set_dir(uint8_t forward)
{
#if MOTOR_DIR_INVERT
    forward = !forward;
#endif
    if (forward) PIOC->PIO_SODR = MOTOR_DIR_PIN;   // DIR = 1
    else         PIOC->PIO_CODR = MOTOR_DIR_PIN;   // DIR = 0
}

void motor_init(void)
{
    // 1. PC23 en sortie PIO classique (MOTOR DIR)
    PMC->PMC_PCER0 = PMC_PCER0_PID13;      // horloge PIOC
    PIOC->PIO_PER  = MOTOR_DIR_PIN;        // contrôlée par le PIO
    PIOC->PIO_OER  = MOTOR_DIR_PIN;        // en sortie
    PIOC->PIO_CODR = MOTOR_DIR_PIN;        // niveau bas au départ

    // 2. PWM canal 0 sur PB12, puissance 0 dès la première période
    current_u = 0;
    pwm_init(PWM_CH_MOTOR, MOTOR_PERIOD_US, duty_to_high_us(0));
}

void motor_set(int32_t u)
{
    // 1. Bornage de la commande
    if (u >  MOTOR_INPUT_MAX) u =  MOTOR_INPUT_MAX;
    if (u < -MOTOR_INPUT_MAX) u = -MOTOR_INPUT_MAX;
    current_u = u;

    // 2. Sens = signe de u
    set_dir(u >= 0);

    // 3. Puissance = |u| ramené en %, limité à MOTOR_MAX_DUTY
    uint32_t duty = (uint32_t)(u >= 0 ? u : -u) * 100 / MOTOR_INPUT_MAX;
    if (duty > MOTOR_MAX_DUTY) duty = MOTOR_MAX_DUTY;

    pwm_set_high_us(PWM_CH_MOTOR, duty_to_high_us(duty));
}

void motor_stop(void)
{
    motor_set(0);
}

int32_t motor_get(void)
{
    return current_u;
}