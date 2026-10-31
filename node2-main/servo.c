#include "servo.h"
#include "pwm.h"

static uint32_t current_us = SERVO_CENTER_US;

static uint32_t clamp_pulse(uint32_t us)
{
    if (us < SERVO_MIN_US) return SERVO_MIN_US;
    if (us > SERVO_MAX_US) return SERVO_MAX_US;
    return us;
}

void servo_init(void)
{
    current_us = SERVO_CENTER_US;
    // Le temps haut initial est écrit avant l'activation du canal :
    // la toute première impulsion est déjà valide (1,5 ms).
    pwm_init(SERVO_PERIOD_US, current_us);
}

void servo_set_pulse_us(uint32_t us)
{
    // Barrière de sécurité unique : tout passe par ici.
    current_us = clamp_pulse(us);
    pwm_set_high_us(current_us);
}

void servo_set_position(int32_t u)
{
    // 1re sécurité : borner l'entrée (évite aussi tout débordement)
    if (u >  SERVO_INPUT_MAX) u =  SERVO_INPUT_MAX;
    if (u < -SERVO_INPUT_MAX) u = -SERVO_INPUT_MAX;

#if SERVO_INVERT
    u = -u;
#endif

    // Mise à l'échelle linéaire : ±100 -> ±600 µs autour de 1500 µs
    const int32_t half_range = (SERVO_MAX_US - SERVO_MIN_US) / 2;   // 600
    int32_t us = SERVO_CENTER_US + (u * half_range) / SERVO_INPUT_MAX;

    // 2e sécurité : bornage final en µs
    servo_set_pulse_us((uint32_t)us);
}

uint32_t servo_get_pulse_us(void)
{
    return current_us;
}
