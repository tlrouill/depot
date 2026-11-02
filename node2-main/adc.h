#pragma once
#include <stdint.h>

/*
 * Driver ADC interne du SAM3X (exercice 7, fin : détection de but).
 *
 * L'ADC tourne en mode "free run" : il convertit le canal en continu,
 * on lit simplement la dernière valeur quand on en a besoin.
 *
 * Canal utilisé : A0 de l'Arduino Due = PA16 = canal AD7.
 * Si ta photodiode IR est branchée sur une autre broche analogique,
 * change ADC_IR_CHANNEL (A1 = AD6, A2 = AD5, A3 = AD4, ... A7 = AD0).
 *
 * Résolution : 12 bits -> 0 .. 4095  (0 V .. 3,3 V)
 */

#define ADC_IR_CHANNEL   7      // A0

void     adc_init(void);
uint16_t adc_read(void);        // dernière conversion du canal IR (0..4095)
