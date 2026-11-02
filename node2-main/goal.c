#include "goal.h"
#include "adc.h"

static uint16_t buffer[GOAL_FILTER_N];
static uint8_t  idx = 0;
static uint32_t sum = 0;

static volatile uint16_t filtered  = 0;
static volatile uint8_t  blocked   = 0;
static volatile uint32_t count     = 0;
static volatile uint8_t  new_goal  = 0;
static uint8_t           below_cnt = 0;
static uint16_t          holdoff   = 0;

void goal_init(void)
{
    adc_init();

    // Petite attente pour que l'ADC ait fait au moins une conversion
    for (volatile uint32_t i = 0; i < 10000; i++) {}

    // Remplit le filtre avec la valeur actuelle (sinon faux but au démarrage)
    uint16_t v = adc_read();
    sum = 0;
    for (uint8_t i = 0; i < GOAL_FILTER_N; i++) {
        buffer[i] = v;
        sum += v;
    }
    idx       = 0;
    filtered  = v;
    blocked   = (v < GOAL_THRESHOLD_LOW);  // si coupé au démarrage, pas de but
    below_cnt = 0;
    holdoff   = 0;
    count     = 0;
    new_goal  = 0;
}

void goal_update(void)
{
    // 1. Moyenne glissante
    uint16_t v = adc_read();
    sum -= buffer[idx];
    buffer[idx] = v;
    sum += v;
    idx = (idx + 1) % GOAL_FILTER_N;
    filtered = (uint16_t)(sum / GOAL_FILTER_N);

    if (holdoff > 0) holdoff--;

    // 2-5. Hystérésis + anti-rebond + détection de front
    if (!blocked) {
        if (filtered < GOAL_THRESHOLD_LOW) {
            if (++below_cnt >= GOAL_DEBOUNCE) {
                blocked   = 1;
                below_cnt = 0;
                if (holdoff == 0) { // hors temps mort : c'est un vrai but
                    count++;        // front libre -> coupé : un but
                    new_goal = 1;
                    holdoff  = GOAL_HOLDOFF;
                }
            }
        } else {
            below_cnt = 0;
        }
    } else {
        // On ne réarme qu'une fois le faisceau franchement rétabli
        if (filtered > GOAL_THRESHOLD_HIGH) {
            blocked = 0;
        }
    }
}

uint8_t goal_is_blocked(void)     { return blocked; }
uint16_t goal_get_filtered(void)  { return filtered; }
uint32_t goal_get_count(void)     { return count; }

void goal_reset_count(void)
{
    count    = 0;
    new_goal = 0;
}

uint8_t goal_new_goal(void)
{
    if (new_goal) {
        new_goal = 0;
        return 1;
    }
    return 0;
}
