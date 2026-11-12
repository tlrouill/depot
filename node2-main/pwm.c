#include "sam.h"
#include "pwm.h"

#define PWM_NB_CH 2     // canaux gérés par ce driver : 0 et 1

static uint32_t period_us[PWM_NB_CH] = {0};
static uint8_t  clock_ready = 0;

// Broche PIOB associée à chaque canal (sortie PWMHx, périphérique B)
static const uint32_t ch_pin[PWM_NB_CH] = {
    PIO_PB12B_PWMH0,    // canal 0 -> PB12 (moteur)
    PIO_PB13B_PWMH1,    // canal 1 -> PB13 (servo)
};

static void pwm_clock_init(void)  // AI written 
{
    // Horloge du périphérique PWM (ID 36 -> PMC_PCER1, bit 4)
    PMC->PMC_PCER1 = PMC_PCER1_PID36;

    // CLKA = MCK / (DIVA * 2^PREA) = 84 MHz / 84 = 1 MHz.
    // Écrite une seule fois : la modifier changerait les autres canaux.
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);

    clock_ready = 1;
}

void pwm_init(uint8_t ch, uint32_t period, uint32_t high_us) // AI written 
{
    if (ch >= PWM_NB_CH) return;

    if (!clock_ready) pwm_clock_init();

    if (period > PWM_MAX_PERIOD_US) period = PWM_MAX_PERIOD_US;
    if (high_us > period)           high_us = period;
    period_us[ch] = period;

    // 1. Broche rendue au périphérique B
    PIOB->PIO_PDR   = ch_pin[ch];
    PIOB->PIO_ABSR |= ch_pin[ch];

    // 2. Canal arrêté pendant la configuration
    PWM->PWM_DIS = (1u << ch);
    while (PWM->PWM_SR & (1u << ch)) {}

    // 3. Horloge CLKA, aligné à gauche,
    //    CPOL = 1 -> la sortie commence HAUTE et reste haute pendant CDTY ticks
    PWM->PWM_CH_NUM[ch].PWM_CMR  = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[ch].PWM_CPRD = period;
    PWM->PWM_CH_NUM[ch].PWM_CDTY = high_us;

    // 4. Démarrage
    PWM->PWM_ENA = (1u << ch);
}

void pwm_set_high_us(uint8_t ch, uint32_t high_us)
{
    if (ch >= PWM_NB_CH) return;
    if (high_us > period_us[ch]) high_us = period_us[ch];

    PWM->PWM_CH_NUM[ch].PWM_CDTYUPD = high_us;
}

uint32_t pwm_get_period_us(uint8_t ch)
{
    return (ch < PWM_NB_CH) ? period_us[ch] : 0;
}