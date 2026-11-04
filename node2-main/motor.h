#pragma once
#include <stdint.h>

/*
 * Driver moteur (shield TTK4155, DRV8871 + porte NOR).
 *
 *   MOTOR PWM -> PB12 (D20), PWM canal 0  : puissance (rapport cyclique)
 *   MOTOR DIR -> PC23 (D7),  sortie PIO   : sens
 *
 * Même horloge que le servo (CLKA = 1 MHz, 1 tick = 1 µs).
 * Période 100 µs -> 10 kHz (< 200 kHz exigés par le shield)
 * et 100 pas de réglage : un temps haut de |u| µs = |u| % de puissance.
 */

#define MOTOR_PERIOD_US   100     // 10 kHz
#define MOTOR_INPUT_MAX   100     // commande attendue : -100 .. +100

// Limite de puissance (en %). Baisse-la pour les premiers essais.
#define MOTOR_MAX_DUTY    100

// Mettre à 1 si le moteur tourne dans le mauvais sens
#define MOTOR_DIR_INVERT    0

// Mettre à 1 si, avec une commande de 0, le moteur part à fond
// (PWM inversé par la porte NOR)
#define MOTOR_PWM_INVERT    0

void motor_init(void);            // démarre moteur arrêté

// u dans [-100, 100] : signe = sens, |u| = puissance en %
void motor_set(int32_t u);

void motor_stop(void);

int32_t motor_get(void);          // dernière commande appliquée