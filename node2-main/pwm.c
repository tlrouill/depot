#include "sam.h"
#include "pwm.h"

static uint32_t period_us = 0;

void pwm_init(uint32_t period, uint32_t high_us)
{
    if (period > PWM_MAX_PERIOD_US) period = PWM_MAX_PERIOD_US;
    if (high_us > period)           high_us = period;
    period_us = period;

    // 1. Horloge du périphérique PWM (ID 36 -> PMC_PCER1, bit 4)
    PMC->PMC_PCER1 = PMC_PCER1_PID36;

    // 2. PB13 rendue au périphérique B (= PWMH1)
    PIOB->PIO_PDR   = PIO_PB13;     // le PIO lâche la broche
    PIOB->PIO_ABSR |= PIO_PB13;     // 1 = périphérique B

    // 3. Canal arrêté pendant la configuration
    PWM->PWM_DIS = (1u << PWM_CHANNEL);
    while (PWM->PWM_SR & (1u << PWM_CHANNEL)) {}

    // 4. CLKA = MCK / (DIVA * 2^PREA) = 84 MHz / 84 = 1 MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);

    // 5. Mode du canal : horloge CLKA, aligné à gauche (CALG = 0),
    //    CPOL = 1 -> la sortie commence HAUTE et reste haute pendant CDTY ticks.
    //    (Avec CPOL = 0 ce serait l'inverse : CDTY = temps BAS.)
    PWM->PWM_CH_NUM[PWM_CHANNEL].PWM_CMR  = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[PWM_CHANNEL].PWM_CPRD = period;     // période en µs
    PWM->PWM_CH_NUM[PWM_CHANNEL].PWM_CDTY = high_us;    // temps haut initial

    // 6. Démarrage
    PWM->PWM_ENA = (1u << PWM_CHANNEL);
}

void pwm_set_high_us(uint32_t high_us)
{
    if (high_us > period_us) high_us = period_us;

    // CDTYUPD : la nouvelle valeur est copiée dans CDTY au début
    // de la période suivante -> pas de glitch sur le signal.
    PWM->PWM_CH_NUM[PWM_CHANNEL].PWM_CDTYUPD = high_us;
}

uint32_t pwm_get_period_us(void)
{
    return period_us;
}