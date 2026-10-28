#include "sam.h"
#include "pwm.h"

#include <stdint.h>


void pwm_init(void) {
    // 1. Horloge du périphérique PWM (ID 36 -> bit 4 de PCER1)
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;

    // 2. PB13 contrôlée par le périphérique B (PWMH1)
    PIOB->PIO_PDR  = PIO_PB13;    // retire la broche au PIO
    PIOB->PIO_ABSR |= PIO_PB13;   // 1 = périphérique B

    // 3. CLKA = MCK / 84 = 1 MHz
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);

    // 4. Canal : horloge CLKA, démarre à l'état haut, aligné à gauche
    PWM->PWM_CH_NUM[SERVO_CH].PWM_CMR  = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[SERVO_CH].PWM_CPRD = SERVO_PERIOD_US;
    PWM->PWM_CH_NUM[SERVO_CH].PWM_CDTY = 1500;   // position neutre

    // 5. Démarrage
    PWM->PWM_ENA = PWM_ENA_CHID1;
}

void pwm_set_pulse_us(uint32_t us) {
    // Ne jamais sortir de la plage du servo
    if (us < SERVO_MIN_US) us = SERVO_MIN_US;
    if (us > SERVO_MAX_US) us = SERVO_MAX_US;

    // Mise à jour prise en compte à la fin de la période (pas de glitch)
    PWM->PWM_CH_NUM[SERVO_CH].PWM_CDTYUPD = us;
}