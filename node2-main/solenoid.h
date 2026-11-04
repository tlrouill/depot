#pragma once
#include <stdint.h>

/*
 * Driver solénoïde (exercice 3.8.4, point 6).
 *
 * Broche de commande -> 1 kΩ -> transistor -> bobine du relais (5-4)
 * Relais : +12 V sur 3, solénoïde sur 1.
 *
 * L'impulsion est NON bloquante : solenoid_fire() active la sortie,
 * solenoid_update() (appelée toutes les 10 ms depuis le TC0) la relâche
 * après SOL_PULSE_TICKS. Le régulateur moteur continue donc de tourner
 * pendant le tir.
 */

// ---- Broche utilisée : D51 = PC12 (change ici si besoin) ----
#define SOL_PIO        PIOC
#define SOL_PIO_ID     ID_PIOC
#define SOL_PIN        PIO_PC12

// ---- Logique du transistor ----
// 1 : PNP MPSA56, émetteur à +3,3 V (figure 28) -> broche à 0 = tir
// 0 : NPN côté masse (bobine en 5 V)            -> broche à 1 = tir
#define SOL_ACTIVE_LOW     1

// ---- Durées (en ticks de 10 ms) ----
#define SOL_PULSE_TICKS    3     // 30 ms d'impulsion : juste assez pour frapper
#define SOL_COOLDOWN_TICKS 30    // 300 ms minimum entre deux tirs

void    solenoid_init(void);     // sortie au repos (avant toute autre chose)
uint8_t solenoid_fire(void);     // lance un tir ; 0 si un tir/cooldown est en cours
void    solenoid_update(void);   // à appeler toutes les 10 ms (TC0)
uint8_t solenoid_is_active(void);