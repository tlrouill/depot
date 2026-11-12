#pragma once
#include <stdint.h>

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