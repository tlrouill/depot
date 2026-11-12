#pragma once
#include <stdint.h>


#define SERVO_PERIOD_US   20000
#define SERVO_MIN_US        900
#define SERVO_CENTER_US    1500
#define SERVO_MAX_US       2100

#define SERVO_INPUT_MAX     100     // entrée attendue : -100 .. +100


void servo_init(void);                  // démarre au centre


void servo_set_position(int32_t u);


void servo_set_pulse_us(uint32_t us);

uint32_t servo_get_pulse_us(void);
