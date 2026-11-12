#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "joystick.h"
#include "servo.h"
#include "tc.h"
#include "goal.h"
#include "encoder.h"
#include "motor.h"
#include "position.h"
#include "controller.h"
#include "solenoid.h"


/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

#define JOG_SPEED      200.0f  // vitesse de la consigne joystick à fond (unités/s)
#define JOY_DEADZONE   15      // |x| < 10 : joystick considéré au centre
#define JOG_MAX_LEAD   50      // la consigne reste à moins de 30 du chariot
#define JOG_LIMIT      92.0f    // marge : la raquette ne touche jamais les butées
#define NEW_GAME_CAN_ID  0x20   // nœud 1 -> nœud 2 : reset de la partie
#define GOAL_CAN_ID      0x30   // nœud 2 -> nœud 1 : [0] buts, [1..2] secondes

static volatile int32_t joy_x = 0;
static float jog_target = 0.0f;
static volatile uint8_t jog_active = 0;   // 1 une fois le régulateur activé

// Incrémenté toutes les 10 ms par l'interruption du TC0
static volatile uint32_t ticks = 0;
static uint8_t last_btn = 0;
 
static void on_tick(void)
{
    ticks++;
    goal_update();        // lecture IR + détection de but toutes les 10 ms

   if (!jog_active) {
        // Calibration en cours : la consigne suit le chariot, sans le piloter
        jog_target = (float)position_read();
    } else {
        int32_t x = joy_x;
        if (x > -JOY_DEADZONE && x < JOY_DEADZONE) x = 0;

        // Position et consigne sont en ticks ; les réglages JOG_* restent en
        // "unités" (-100..100) et sont convertis avec unit (~28 ticks/unité)
        float unit = position_unit();
        jog_target += JOG_SPEED * unit * (float)x / 100.0f * 0.010f;

        float pos = (float)position_read();
        if (jog_target > pos + JOG_MAX_LEAD * unit) jog_target = pos + JOG_MAX_LEAD * unit;
        if (jog_target < pos - JOG_MAX_LEAD * unit) jog_target = pos - JOG_MAX_LEAD * unit;

              
        float lo = (100.0f - JOG_LIMIT) * unit;          
        float hi = (float)position_span() - lo;
        if (jog_target > hi) jog_target = hi;
        if (jog_target < lo) jog_target = lo;
        controller_set_target((int32_t)jog_target);
    }

    controller_update();  // régulateur PI de position, T = 10 ms
    //motor_set(joy_x * 60 / 100);   // TEST : joystick -> moteur directement
    solenoid_update();
}




int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    
    CanInit init = {
        .phase2 = 1,   // Phase 2 = (1 + 1) = 2 Tq
        .propag = 6,   // Propag  = (1 + 1) = 2 Tq
        .phase1 = 5,   // Phase 1 = (2 + 1) = 3 Tq
        .sjw    = 0,   // SJW     = (0 + 1) = 1 Tq
        .brp    = 41,  // Prescaler BRP
        .smp    = 0    // Échantillonnage unique
    };

    can_init(init, 0);

    CanMsg m;
    JoystickData js = {0};
 
    servo_init();                       // PWM 20 ms, servo au centre (1,5 ms)
    goal_init();                        // ADC + filtre IR (AVANT le timer)
    tc_periodic_init(10000, on_tick);   // interruption toutes les 10 ms
    encoder_init();
    motor_init();                      // moteur arrêté
    solenoid_init();


       // Va aux deux butées : -100 / +100, centre = 0 (timer déjà lancé)
    if (position_calibrate() == 0) {
        controller_enable();            // à partir d'ici, seul le PI pilote le moteur
        jog_active = 1;                 // le joystick peut maintenant déplacer la consigne
    } else {
        printf("Regulateur NON active (calibration ratee)\n\r");
    }

    uint32_t last_print = 0;
    uint8_t  buts_partie = 0;
 
    while (1) {
                // Message du nœud 1 : joystick (0x41) ou reset (0x20)
        if (can_rx(&m)) {
            if (joystick_decode(&m, &js)) {
                joy_x = js.x;
                servo_set_position(js.y);
                if (js.btn && !last_btn) {
                    solenoid_fire();
                }
                last_btn = js.btn;
            } else if (m.id == NEW_GAME_CAN_ID) {
                buts_partie = 0;
            }
        }
 
        // But : on prévient le nœud 1
        if (goal_new_goal()) {
            buts_partie++;
            CanMsg but = {
                .id = GOAL_CAN_ID,
                .length = 3,
                .byte = {buts_partie, },
            };
            // N'envoie que si la boîte d'émission est libre :
            // sans nœud 1, can_tx() attendrait indéfiniment
            if (CAN0->CAN_MB[0].CAN_MSR & CAN_MSR_MRDY) {
                can_tx(but);
            }
        }
 
        // 100 ticks de 10 ms = 1 s : vérifie que le timer tourne
        if (ticks - last_print >= 100) {
            last_print = ticks;
            printf("t = %lu s, consigne = %ld, pos = %ld, u = %ld, pulse = %lu us, ir = %u, buts = %lu\n\r",
                   (unsigned long)(ticks / 100),
                   (long)controller_get_target(),
                   (long)position_read(),
                   (long)controller_get_output(),
                   (unsigned long)servo_get_pulse_us(),
                   goal_get_filtered(),
                   (unsigned long)goal_get_count());
        }
    }



    // //day 6 test 
    // while (1) {
    //     if (joystick_receive(&js)) {
    //         printf("x = %d, y = %d, dir = %d, btn = %d\n\r", js.x, js.y, js.dir, js.btn);
    //     }
    // }

    // while (1) {
    //     if (can_rx(&m)) {
    //         // Un message vient d'être reçu !
    //         printf("CanMsg(id:0x%03X, length:%d, data:{", m.id, m.length);
    //         if (m.length > 0) printf("%d", m.byte[0]);
    //         for (uint8_t i = 1; i < m.length; i++) printf(", %d", m.byte[i]);
    //         printf("})\n\r");
    //     }
    // // Suite du code (ex: contrôle des moteurs, etc.)
    // }
    
}