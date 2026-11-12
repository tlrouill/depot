#pragma once
#include <stdint.h>


#define SOL_PIO        PIOC
#define SOL_PIO_ID     ID_PIOC
#define SOL_PIN        PIO_PC12



// ---- Durées (en ticks de 10 ms) ----
#define SOL_PULSE_TICKS    10     // 30 ms d'impulsion : juste assez pour frapper
#define SOL_COOLDOWN_TICKS 30    // 300 ms minimum entre deux tirs

void    solenoid_init(void);     // sortie au repos (avant toute autre chose)
uint8_t solenoid_fire(void);     // lance un tir ; 0 si un tir/cooldown est en cours
void    solenoid_update(void);   // à appeler toutes les 10 ms (TC0)
uint8_t solenoid_is_active(void);