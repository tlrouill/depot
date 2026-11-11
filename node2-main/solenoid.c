#include "sam.h"
#include "solenoid.h"

static volatile uint8_t  pulse    = 0;   // ticks restants d'impulsion
static volatile uint16_t cooldown = 0;   // ticks restants avant un nouveau tir

static inline void out_active(void)
{
    SOL_PIO->PIO_CODR = SOL_PIN;   // 0 -> transistor PNP passant -> tir
}

static inline void out_idle(void)
{
    SOL_PIO->PIO_SODR = SOL_PIN;   // 1 -> transistor bloqué -> repos
}

void solenoid_init(void)
{
    // 1. Horloge du port
    PMC->PMC_PCER0 = (1u << SOL_PIO_ID);

    // 2. Niveau de repos écrit AVANT de passer en sortie :
    //    sinon le solénoïde peut tirer une fois au démarrage.
    out_idle();

    // 3. Broche gérée par le PIO, en sortie, sans pull-up
    SOL_PIO->PIO_PER  = SOL_PIN;
    SOL_PIO->PIO_PUDR = SOL_PIN;
    SOL_PIO->PIO_OER  = SOL_PIN;

    pulse    = 0;
    cooldown = 0;
}

uint8_t solenoid_fire(void)
{
    if (pulse || cooldown) {
        return 0;                       // déjà en train de tirer / trop tôt
    }
    pulse    = SOL_PULSE_TICKS;
    cooldown = SOL_PULSE_TICKS + SOL_COOLDOWN_TICKS;
    out_active();
    return 1;
}

void solenoid_update(void)
{
    if (pulse) {
        if (--pulse == 0) {
            out_idle();                 // fin de l'impulsion
        }
    }
    if (cooldown) {
        cooldown--;
    }
}

uint8_t solenoid_is_active(void)
{
    return pulse != 0;
}