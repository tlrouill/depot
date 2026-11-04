#pragma once
#include <stdint.h>

/*
 * Driver bas niveau du module PWM du SAM3X.
 *
 * Canaux gérés (sorties PWMH, périphérique B de PIOB) :
 *   canal 0 -> PB12 = PWMH0 = D20 -> MOTOR PWM du shield
 *   canal 1 -> PB13 = PWMH1 = D21 -> signal du servo
 *
 * Horloge commune : CLKA = MCK / 84 = 1 MHz  ->  1 tick = 1 µs
 * pour tous les canaux.
 *
 * Ce driver ne connaît ni le servo ni le moteur : il génère un signal
 * de période donnée avec un temps haut donné. Les limites de sécurité
 * sont dans servo.c et motor.c.
 */

#define PWM_CH_MOTOR       0
#define PWM_CH_SERVO       1

#define PWM_MAX_PERIOD_US  65535    // compteur de canal sur 16 bits

// Configure la broche et le canal, puis le démarre.
// high_us est le temps haut initial (appliqué AVANT d'activer le canal).
// L'horloge CLKA est configurée au premier appel seulement.
void pwm_init(uint8_t ch, uint32_t period_us, uint32_t high_us);

// Change le temps haut. Prise en compte à la fin de la période en cours
// (registre de mise à jour) -> aucune impulsion tronquée.
// Valeur bornée à [0, période].
void pwm_set_high_us(uint8_t ch, uint32_t high_us);

uint32_t pwm_get_period_us(uint8_t ch);