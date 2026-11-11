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
#define JOY_DEADZONE   10      // |x| < 10 : joystick considéré au centre
#define JOG_MAX_LEAD   30      // la consigne reste à moins de 30 du chariot

static volatile int32_t joy_x = 0;
static float jog_target = 0.0f;

// Incrémenté toutes les 10 ms par l'interruption du TC0
static volatile uint32_t ticks = 0;
static uint8_t last_btn = 0;
 
static void on_tick(void)
{
    ticks++;
    goal_update();        // lecture IR + détection de but toutes les 10 ms
    int32_t x = joy_x;
    if (x > -JOY_DEADZONE && x < JOY_DEADZONE) x = 0;      // zone morte

    jog_target += JOG_SPEED * (float)x / 100.0f * 0.010f;  // incrément (T = 10 ms)

    float pos = (float)position_read();                    // pas trop d'avance
    if (jog_target > pos + JOG_MAX_LEAD) jog_target = pos + JOG_MAX_LEAD;
    if (jog_target < pos - JOG_MAX_LEAD) jog_target = pos - JOG_MAX_LEAD;

    if (jog_target >  100.0f) jog_target =  100.0f;        // rester dans le terrain
    if (jog_target < -100.0f) jog_target = -100.0f;

    controller_set_target((int32_t)jog_target);
    controller_update();  // régulateur PI de position, T = 10 ms
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

    // Appel à la fonction
    can_init(init, 0);

    printf("Hello World\n\r");

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
        controller_set_target(0);       // reste au centre au départ
        controller_enable();            // à partir d'ici, seul le PI pilote le moteur
    } else {
        printf("Regulateur NON active (calibration ratee)\n\r");
    }

    uint32_t last_print = 0;
 
    while (1) {
        if (joystick_receive(&js)) {
            joy_x = js.x;
            //controller_set_target(js.x); // axe x -> POSITION voulue du chariot
            servo_set_position(js.y);   // axe y -> servo (0,9..2,1 ms)
            if (js.btn && !last_btn) {      // front montant : on vient d'appuyer
                solenoid_fire();
            }
            last_btn = js.btn;
        }
 
        if (goal_new_goal()) {
            printf("BUT ! total = %lu\n\r", (unsigned long)goal_get_count());
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