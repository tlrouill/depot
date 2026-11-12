#pragma once
#include <stdint.h>

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
