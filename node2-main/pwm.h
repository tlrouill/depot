#pragma once
#include <stdint.h>

#define PWM_CH_MOTOR       0
#define PWM_CH_SERVO       1

#define PWM_MAX_PERIOD_US  65535    // compteur de canal sur 16 bits


void pwm_init(uint8_t ch, uint32_t period_us, uint32_t high_us);


void pwm_set_high_us(uint8_t ch, uint32_t high_us);

uint32_t pwm_get_period_us(uint8_t ch);