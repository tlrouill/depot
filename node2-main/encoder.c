#include "sam.h"
#include "encoder.h"

void encoder_init(void)
{
    // 1. Horloge de TC2 canal 0 (= "TC6", ID 33 -> PMC_PCER1 bit 1)
    PMC->PMC_PCER1 = PMC_PCER1_PID33;

    // 2. PC25 / PC26 rendues au périphérique B (TIOA6 / TIOB6)
    PIOC->PIO_PDR   = PIO_PC25B_TIOA6 | PIO_PC26B_TIOB6;
    PIOC->PIO_ABSR |= PIO_PC25B_TIOA6 | PIO_PC26B_TIOB6;

    // 3. Canal 0 en mode capture, horloge = XC0 (sortie du QDEC).
    //    ETRGEDG/ABETRG : configuration exigée par la datasheet (§36.6.14.4)
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0
                              | TC_CMR_ETRGEDG_RISING
                              | TC_CMR_ABETRG;

    // 4. Décodeur de quadrature :
    //    QDEN   : active le QDEC
    //    POSEN  : mesure de position (le compteur monte/descend selon le sens)
    //    EDGPHA : compte les fronts de A ET de B -> résolution x4
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA
#if ENCODER_INVERT
                | TC_BMR_INVA
#endif
                ;

    // 5. Démarrage + remise à zéro du compteur
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_read(void)
{
    // Le compteur est sur 32 bits : en le lisant en signé,
    // un pas en arrière depuis 0 donne -1 (et non 4294967295).
    return (int32_t)TC2->TC_CHANNEL[0].TC_CV;
}

void encoder_reset(void)
{
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;   // software trigger = CV à 0
}