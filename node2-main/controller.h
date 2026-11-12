#pragma once
#include <stdint.h>



// --- Réglages (à ajuster sur la vraie carte) ---------------------------
#define CTRL_T_MS        10      // période d'appel (= période du timer)
#define CTRL_KP          2.0f    // gain proportionnel
#define CTRL_KI          1.5f    // gain intégral (en 1/s)
#define CTRL_DEADZONE    2       // |erreur| <= ça : on considère qu'on y est
#define CTRL_U_MIN      15       // puissance minimale pour vaincre le frottement
#define CTRL_U_MAX     80       // puissance maximale envoyée au moteur
#define CTRL_I_MAX      50.0f    // limite de la part intégrale (anti-windup)
// -----------------------------------------------------------------------

void    controller_enable(void);          // remet l'intégrale à 0 et démarre
void    controller_disable(void);         // arrête le moteur
void    controller_set_target(int32_t r); // consigne -100..100
void    controller_update(void);          // à appeler toutes les CTRL_T_MS

int32_t controller_get_target(void);
int32_t controller_get_output(void);      // dernière commande moteur