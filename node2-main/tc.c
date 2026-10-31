#include "sam.h"
#include "tc.h"

#define TC_CLK_HZ   (84000000u / 2u)    // TIMER_CLOCK1 = MCK/2

static volatile tc_callback_t callback = 0;

void tc_periodic_init(uint32_t period_us, tc_callback_t cb)
{
    callback = cb;

    // 1. Horloge du périphérique TC0 (ID 27 -> PMC_PCER0, bit 27)
    PMC->PMC_PCER0 = PMC_PCER0_PID27;

    TcChannel *ch = &TC0->TC_CHANNEL[0];

    // 2. Canal arrêté pendant la configuration
    ch->TC_CCR = TC_CCR_CLKDIS;

    // 3. Mode waveform, comptage montant avec remise à 0 sur RC
    ch->TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1
               | TC_CMR_WAVE
               | TC_CMR_WAVSEL_UP_RC;

    // 4. Période : 42 ticks par µs (compteur 32 bits -> jusqu'à ~102 s)
    ch->TC_RC = period_us * (TC_CLK_HZ / 1000000u);

    // 5. Interruption sur "RC compare" uniquement
    ch->TC_IDR = 0xFFFFFFFF;
    ch->TC_IER = TC_IER_CPCS;
    (void)ch->TC_SR;                    // efface un éventuel flag en attente

    NVIC_ClearPendingIRQ(TC0_IRQn);
    NVIC_SetPriority(TC0_IRQn, 3);
    NVIC_EnableIRQ(TC0_IRQn);

    // 6. Activation de l'horloge + remise à zéro/démarrage du compteur
    ch->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void tc_stop(void)
{
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
    NVIC_DisableIRQ(TC0_IRQn);
}

// Remplace le Dummy_Handler (déclaré "weak" dans exceptions.c)
void TC0_Handler(void)
{
    // Lire TC_SR acquitte l'interruption ; sans ça on y reste bloqué.
    uint32_t status = TC0->TC_CHANNEL[0].TC_SR;

    if ((status & TC_SR_CPCS) && callback) {
        callback();
    }
}
