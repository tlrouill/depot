#include "sam.h"
#include "adc.h"

void adc_init(void)
{
    // 1. Horloge du périphérique ADC (ID 37 -> PMC_PCER1, bit 5)
    PMC->PMC_PCER1 = (1u << (ID_ADC - 32));

    // 2. Reset logiciel de l'ADC
    ADC->ADC_CR = ADC_CR_SWRST;

    // 3. Mode :
    //    - FREERUN : conversions en continu, pas besoin de déclencher
    //    - PRESCAL = 41 -> ADCClock = MCK / ((41+1)*2) = 1 MHz (max 22 MHz)
    //    - STARTUP / TRACKTIM : marges confortables, on n'est pas pressé
    ADC->ADC_MR = ADC_MR_FREERUN
                | ADC_MR_PRESCAL(41)
                | ADC_MR_STARTUP_SUT64
                | ADC_MR_TRACKTIM(3)
                | ADC_MR_TRANSFER(1);

    // 4. Pas d'interruption, on lit en polling
    ADC->ADC_IDR = 0xFFFFFFFF;

    // 5. Active uniquement le canal IR (la broche passe automatiquement à l'ADC)
    ADC->ADC_CHDR = 0xFFFF;
    ADC->ADC_CHER = (1u << ADC_IR_CHANNEL);

    // 6. Démarre (en free run, une seule fois suffit)
    ADC->ADC_CR = ADC_CR_START;
}

uint16_t adc_read(void)
{
    return (uint16_t)(ADC->ADC_CDR[ADC_IR_CHANNEL] & ADC_CDR_DATA_Msk);
}
