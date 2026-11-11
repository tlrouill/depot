#pragma once
#include <stdint.h>

/*
 * Détection de but (exercice 7, fin).
 *
 * Quand la balle passe devant le faisceau IR, la photodiode ne reçoit
 * plus de lumière et la tension lue par l'ADC chute.
 *
 * Pour ne compter qu'UN but par passage malgré le bruit :
 *   1. filtre (moyenne glissante sur GOAL_FILTER_N échantillons)
 *   2. hystérésis : bloqué si < GOAL_THRESHOLD_LOW,
 *                   libre   si > GOAL_THRESHOLD_HIGH
 *   3. anti-rebond : il faut GOAL_DEBOUNCE échantillons consécutifs
 *      sous le seuil avant de valider
 *   4. le but n'est compté que sur le front libre -> bloqué
 *   5. temps mort après un but (GOAL_HOLDOFF) : une balle qui rebondit
 *      dans le faisceau ne compte pas plusieurs fois (conseil du manuel)
 *
 * goal_update() doit être appelée périodiquement (ici toutes les 10 ms
 * depuis l'interruption du TC0).
 *
 * !! Les seuils dépendent du montage : lance le programme, regarde la
 * valeur "ir" affichée faisceau libre puis faisceau coupé (doigt / balle),
 * et place les seuils entre les deux.
 */

#define GOAL_THRESHOLD_LOW    300   // en dessous : faisceau coupé
#define GOAL_THRESHOLD_HIGH  600   // au dessus  : faisceau libre
#define GOAL_FILTER_N           4   // taille de la moyenne glissante
#define GOAL_DEBOUNCE           3   // échantillons (x 10 ms) pour valider
#define GOAL_HOLDOFF          100   // x 10 ms = 1 s sans nouveau but après un but

void     goal_init(void);
void     goal_update(void);         // à appeler toutes les 10 ms
uint8_t  goal_is_blocked(void);     // 1 si le faisceau est coupé en ce moment
uint16_t goal_get_filtered(void);   // valeur filtrée (pour calibrer les seuils)
uint32_t goal_get_count(void);      // nombre de buts depuis le début


// Retourne 1 une seule fois par nouveau but (puis 0 jusqu'au suivant)
uint8_t  goal_new_goal(void);
