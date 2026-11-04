#pragma once
#include <stdint.h>

/*
 * Position du chariot, calibrée sur [-100, 100].
 *
 * position_calibrate() :
 *   1. pousse le moteur dans le sens négatif jusqu'à la butée -> -100
 *   2. pousse le moteur dans le sens positif jusqu'à la butée -> +100
 *   3. calcule le centre (= 0) et ramène le chariot au centre
 *
 * La butée est détectée quand l'encodeur ne bouge presque plus
 * pendant un moment (moteur bloqué).
 *
 * Prérequis : motor_init(), encoder_init() et tc_periodic_init()
 * doivent avoir été appelés (le timer sert à mesurer le temps).
 */

// --- Réglages (à ajuster sur la vraie carte) ---------------------------
#define CAL_SPEED          20     // puissance pendant la calibration (%)
#define CAL_CHECK_MS       50     // intervalle entre deux lectures encodeur
#define CAL_STALL_TICKS     5     // bouge de moins que ça -> "immobile"
#define CAL_STALL_CHECKS    4     // immobile N fois de suite -> butée (200 ms)
#define CAL_START_MS      300     // laisse le moteur démarrer avant de tester
#define CAL_TIMEOUT_MS   5000     // abandon si aucune butée trouvée
#define CAL_MIN_SPAN      100     // course minimale (ticks) pour être valide
// -----------------------------------------------------------------------

// Retourne 0 si OK, -1 en cas d'échec (moteur ou encodeur qui ne répond pas)
int position_calibrate(void);

// Position actuelle dans [-100, 100] (0 = centre). 0 si pas calibré.
int32_t position_read(void);

uint8_t position_is_calibrated(void);