#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "joystick.h"


/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

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


    JoystickData js;

    while (1) {
        if (joystick_receive(&js)) {
            printf("x = %d, y = %d, dir = %d, btn = %d\n\r", js.x, js.y, js.dir, js.btn);
        }
    }

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