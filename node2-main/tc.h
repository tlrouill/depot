#pragma once
#include <stdint.h>

/*
 * Driver Timer/Counter (exercice 3.7.7, point 1) : interruption périodique.
 *
 * Utilise TC0 canal 0 en mode "waveform", UP_RC : le compteur monte de 0
 * à RC, déclenche une interruption, puis repart à 0.
 * Horloge : TIMER_CLOCK1 = MCK / 2 = 42 MHz  ->  RC = période_µs * 42.
 *
 * Servira pour le régulateur (exercice suivant) : on appelle la loi de
 * commande à intervalle fixe depuis le callback.
 */

typedef void (*tc_callback_t)(void);

// Lance une interruption toutes les period_us microsecondes ;
// cb est appelé depuis l'interruption (doit être court !).
void tc_periodic_init(uint32_t period_us, tc_callback_t cb);

void tc_stop(void);
