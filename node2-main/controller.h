#pragma once
#include <stdint.h>

/*
 * Régulateur PI de position du chariot (exercice 3.8.4, point 4).
 *
 *   consigne  r  : position voulue, -100 .. 100 (le joystick)
 *   mesure    y  : position_read(),  -100 .. 100 (encodeur calibré)
 *   erreur    e  = r - y
 *   commande  u  = Kp*e + T*Ki*somme(e)   -> motor_set(u)
 *
 * controller_update() doit être appelée toutes les CTRL_T_MS ms,
 * depuis l'interruption du timer (on_tick dans main.c).
 * Pendant que le régulateur est actif, SEUL lui doit appeler motor_set().
 */

// --- Réglages (à ajuster sur la vraie carte) ---------------------------
#define CTRL_T_MS        10      // période d'appel (= période du timer)
#define CTRL_KP          1.0f    // gain proportionnel
#define CTRL_KI          1.5f    // gain intégral (en 1/s)
#define CTRL_DEADZONE    2       // |erreur| <= ça : on considère qu'on y est
#define CTRL_U_MIN      18       // puissance minimale pour vaincre le frottement
#define CTRL_U_MAX     80       // puissance maximale envoyée au moteur
#define CTRL_I_MAX      50.0f    // limite de la part intégrale (anti-windup)
// -----------------------------------------------------------------------

void    controller_enable(void);          // remet l'intégrale à 0 et démarre
void    controller_disable(void);         // arrête le moteur
void    controller_set_target(int32_t r); // consigne -100..100
void    controller_update(void);          // à appeler toutes les CTRL_T_MS

int32_t controller_get_target(void);
int32_t controller_get_output(void);      // dernière commande moteur