#pragma once
#include <stdint.h>

/*
 * Driver encodeur du moteur (exercice 3.8.4, point 1).
 *
 * Utilise le décodeur de quadrature matériel (QDEC) du bloc TC2 :
 *   signal A -> PC25 = TIOA6   (périphérique B)
 *   signal B -> PC26 = TIOB6   (périphérique B)
 * La position (en "ticks", décodage x4) est dans TC2 canal 0 -> TC_CV.
 *
 * L'encodeur est incrémental : la position vaut 0 au démarrage,
 * quelle que soit la position réelle du chariot -> il faut calibrer.
 */

// Mettre à 1 si la position diminue quand le chariot va dans le sens "positif"
#define ENCODER_INVERT 0

void    encoder_init(void);

// Position actuelle (signée), en ticks depuis le dernier encoder_reset()
int32_t encoder_read(void);

// Remet le compteur à 0 (la position actuelle devient la référence)
void    encoder_reset(void);