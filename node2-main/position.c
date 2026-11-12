#include <stdio.h>
#include "position.h"
#include "encoder.h"
#include "motor.h"
#include "tc.h"

static int32_t center = 0;      // valeur encodeur au milieu de la course
static int32_t half_span = 0;   // (butée + - butée -) / 2, signé
static uint8_t calibrated = 0;

static int32_t abs32(int32_t v) { return v < 0 ? -v : v; }


static int drive_to_end(int32_t u, int32_t *end_pos)
{
    uint32_t start = tc_millis();
    int32_t  last  = encoder_read();
    uint8_t  still = 0;

    motor_set(u);

    while (tc_millis() - start < CAL_TIMEOUT_MS) {
        tc_delay_ms(CAL_CHECK_MS);

        int32_t now = encoder_read();

        // Pendant le démarrage, on ne teste pas (le moteur accélère)
        if (tc_millis() - start >= CAL_START_MS) {
            if (abs32(now - last) < CAL_STALL_TICKS) {
                if (++still >= CAL_STALL_CHECKS) {
                    motor_stop();
                    *end_pos = now;
                    return 0;               // butée atteinte
                }
            } else {
                still = 0;                  // ça bouge encore
            }
        }
        last = now;
    }

    motor_stop();
    return -1;                              // timeout
}

// Ramène le chariot vers une position cible (en -100..100), régulation P simple
static void go_to(int32_t target)
{
    uint32_t start = tc_millis();

    while (tc_millis() - start < 3000) {
        // Erreur convertie en unités (-100..100) : position_read() est en ticks
        float err = (float)(target - position_read()) / position_unit();
        if (err > -3.0f && err < 3.0f) break;   // assez proche

        int32_t u = (int32_t)(err * 2.0f);      // gain P
        if (u >  CAL_SPEED) u =  CAL_SPEED;
        if (u < -CAL_SPEED) u = -CAL_SPEED;
        if (u > 0 && u <  20) u =  20;      // minimum pour vaincre le frottement
        if (u < 0 && u > -20) u = -20;

        motor_set(u);
        tc_delay_ms(10);
    }
    motor_stop();
}

int position_calibrate(void) // AI written 
{
    int32_t end_neg, end_pos;
    calibrated = 0;

    printf("Calibration : butee negative...\n\r");
    if (drive_to_end(-CAL_SPEED, &end_neg) != 0) {
        printf("Calibration ECHEC : pas de butee negative (timeout)\n\r");
        return -1;
    }
    tc_delay_ms(200);                       // laisse le chariot se poser

    printf("Calibration : butee positive...\n\r");
    if (drive_to_end(+CAL_SPEED, &end_pos) != 0) {
        printf("Calibration ECHEC : pas de butee positive (timeout)\n\r");
        return -1;
    }
    tc_delay_ms(200);

    // Si l'encodeur n'a presque pas bougé, quelque chose ne va pas
    // (moteur pas alimenté, câble encodeur débranché, PWM inversé...)
    if (abs32(end_pos - end_neg) < CAL_MIN_SPAN) {
        printf("Calibration ECHEC : course trop petite (%ld ticks)\n\r",
               (long)(end_pos - end_neg));
        return -1;
    }

    // Centre = milieu des deux butées ; half_span garde son signe,
    // donc ça marche quel que soit le sens de comptage de l'encodeur.
    center    = (end_neg + end_pos) / 2;
    half_span = (end_pos - end_neg) / 2;
    calibrated = 1;

    printf("Calibration OK : -100 = %ld, +100 = %ld, centre = %ld ticks\n\r",
           (long)end_neg, (long)end_pos, (long)center);

        go_to(position_span() / 2);             // retour au centre (milieu de 0..n)
    return 0;
}

int32_t position_read(void)
{
    if (!calibrated) return 0;

    // Position en ticks depuis le centre, positive vers la butée "+100"
    // (le signe de half_span corrige le sens de comptage de l'encodeur)
    int32_t p = encoder_read() - center;
    // Origine sur la butée "-100" : 0 = butée gauche, position_span() = butée droite
    int32_t half = (half_span < 0) ? -half_span : half_span;
    return ((half_span < 0) ? -p : p) + half;
}

float position_unit(void)
{
    // Nombre de ticks pour 1 "unité" de l'ancienne échelle -100..100
    // (ex. demi-course de 2800 ticks -> 28 ticks par unité)
    int32_t half = (half_span < 0) ? -half_span : half_span;
    return calibrated ? (float)half / 100.0f : 1.0f;
}

uint8_t position_is_calibrated(void)
{
    return calibrated;
}

int32_t position_span(void)
{
    int32_t half = (half_span < 0) ? -half_span : half_span;
    return 2 * half;
}