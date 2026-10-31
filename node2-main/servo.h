#pragma once
#include <stdint.h>

/*
 * Driver servo (exercice 3.7.7, point 2).
 *
 * Prend une consigne (sortie du régulateur / position joystick) et calcule
 * le temps haut à envoyer au driver PWM.
 *   période      : 20 ms
 *   centre       : 1,5 ms
 *   plage valide : 0,9 ms .. 2,1 ms  (JAMAIS en dehors)
 */

#define SERVO_PERIOD_US   20000
#define SERVO_MIN_US        900
#define SERVO_CENTER_US    1500
#define SERVO_MAX_US       2100

#define SERVO_INPUT_MAX     100     // entrée attendue : -100 .. +100

// Mettre à 1 si le servo tourne dans le mauvais sens par rapport au joystick
#define SERVO_INVERT          0

void servo_init(void);                  // démarre au centre

// u dans [-100, 100] : -100 -> 0,9 ms, 0 -> 1,5 ms, +100 -> 2,1 ms
void servo_set_position(int32_t u);

// Accès direct en µs (toujours borné à [900, 2100])
void servo_set_pulse_us(uint32_t us);

uint32_t servo_get_pulse_us(void);
