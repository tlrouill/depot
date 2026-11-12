#pragma once
#include <stdint.h>



#define MOTOR_PERIOD_US   100     // 10 kHz
#define MOTOR_INPUT_MAX   100     // commande attendue : -100 .. +100
#define MOTOR_MAX_DUTY    100


void motor_init(void);            // démarre moteur arrêté

void motor_set(int32_t u);

void motor_stop(void);

