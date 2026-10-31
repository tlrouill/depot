#pragma once
#include <stdint.h>

/*
 * Driver bas niveau du module PWM du SAM3X (exercice 3.7.7, point 1).
 *
 * Sortie : PB13 = PWMH1 (périphérique B), reliée au header servo du shield.
 * Horloge : CLKA = MCK / 84 = 1 MHz  ->  1 tick = 1 µs.
 *
 * Ce driver ne connaît PAS les limites du servo : il génère juste un signal
 * de période donnée avec un temps haut donné. Les limites de sécurité sont
 * dans servo.c (point 2).
 */

#define PWM_CHANNEL        1        // canal 1 -> PWMH1 sur PB13
#define PWM_MAX_PERIOD_US  65535    // compteur de canal sur 16 bits

// Configure l'horloge, la broche et le canal, puis démarre le PWM.
// high_us est le temps haut initial (appliqué AVANT d'activer le canal).
void pwm_init(uint32_t period_us, uint32_t high_us);

// Change le temps haut. Prise en compte à la fin de la période en cours
// (registre de mise à jour), donc aucune impulsion tronquée ou doublée.
// Valeur bornée à [0, période].
void pwm_set_high_us(uint32_t high_us);

uint32_t pwm_get_period_us(void);