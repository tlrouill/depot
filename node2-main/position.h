#pragma once
#include <stdint.h>




#define CAL_SPEED          20     // puissance pendant la calibration (%)
#define CAL_CHECK_MS       50     // intervalle entre deux lectures encodeur
#define CAL_STALL_TICKS     5     // bouge de moins que ça -> "immobile"
#define CAL_STALL_CHECKS    4     // immobile N fois de suite -> butée (200 ms)
#define CAL_START_MS      300     // laisse le moteur démarrer avant de tester
#define CAL_TIMEOUT_MS   5000     // abandon si aucune butée trouvée
#define CAL_MIN_SPAN      100     // course minimale (ticks) pour être valide



int position_calibrate(void);

int32_t position_read(void);

float position_unit(void);

uint8_t position_is_calibrated(void);

int32_t position_span(void);   