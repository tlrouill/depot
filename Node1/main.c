#include "system_definitions.h"

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart_driver.h"
#include "external_memory_driver.h"
#include "sram_test.h"
#include "adc_driver.h"
#include "spi_driver.h"
#include "oled_driver.h"
#include "io_interface_driver.h"
#include "menu.h"
#include "interrupts.h"
#include "can_driver.h"
#include "MCP2515_driver.h"

#define WAVE_PIN PB1
#define ERROR_LED PB0

volatile uint8_t *address = (volatile uint8_t *)0x1800;

static FILE uartstdout = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main(void) {
    set_bit(DDRB, ERROR_LED);
    /*
    set_bit(DDRB, WAVE_PIN);


    //Day 1 initial test
    while(1) {
        set_bit(PORTB, WAVE_PIN);
        _delay_ms(500);
        clear_bit(PORTB, WAVE_PIN);
        _delay_ms(500);
    }*/
    uart_init();
    stdout = stdin = &uartstdout; // Replace the defualt stdout/in stream with the custom uart one


    //Day 1 test
    /*while(1) { 
        

        //char c[32] = getchar(); // Waits until it gets a character on the stdin stream
        char c[32];
        fgets(c, sizeof(c), stdin);
        printf("Hello, PC! You've sent me this: %s\n", c); 
    }
        */

        
    external_memory_init();
    //SRAM_test(); //Day 2 SRAM test

    //Day 2 final test
    /*while(1) {
        address[1]=0;
        _delay_ms(1000);
        address_ADC[2]=1;
        _delay_ms(1000);

        for (int i=0; i<=10000; i++){
        address[i]=1+i;
        _delay_ms(1000);
       }
    }*/

    init_adc();
    //Day 3 test 
    /*while(1){
        address_ADC[0]=1;
        _delay_us(15);
        uint8_t digital_signal_slider_x =read_ADC(0);
        uint8_t digital_signal_slider_y =read_ADC(1);
        uint8_t digital_signal_y =read_ADC(2);
        uint8_t digital_signal_x = read_ADC(3);

        int pos_x = master_conversion_x(digital_signal_x);
        int pos_y = master_conversion_y(digital_signal_y);
        int pos_slider_x = voltage_conversion_slider(digital_signal_slider_x);
        int pos_slider_y  = voltage_conversion_slider(digital_signal_slider_y);

        direction joystick = joystick_position(pos_x, pos_y);

        printf("x: %i\n", pos_x);
        printf("y: %i\n", pos_y);
        printf( " direction : %d\n", joystick);

        //printf("x slider: %i\n", pos_slider_x);
        //printf("y slider: %i\n", pos_slider_y);

        _delay_ms(1000);
    }*/ 
    SPI_init();
    _delay_ms(10);
    oled_init();

    // io_set_led(0, 1);
    // io_set_led(1, 1);
    // io_set_led(2, 1);
    // io_set_led(3, 1);
    // io_set_led(4, 1);
    // io_set_led(5, 1);

    // while (1)
    // {
    //      Buttons btns = io_read_buttons();
    //     printf("Bouton up : %d\n", btns.L2);
    //     _delay_ms(500);
    // }
    
   
    //oled_run_simple_menu();
    /*while (1)
    {
        io_update_led_from_buttons();
        _delay_ms(150);
    }*/

    //miso test
    // while (1) {
    //     io_read_buttons();
    //     _delay_ms(100);
    // }

    CAN_init();

    
    init_interruptions();


    //test day 5 
    
    Can_message msg_tx = { .id = 0x041, .length = 2, .data = {0xAA, 0x55,} };
    // Can_message msg_rx = {0};

    // // Debug : mode attendu 0x40 (loopback)
    // printf("CANSTAT = %02x\n", MCP_read(REG_CANSTAT));

    // CAN_send_message(msg_tx);
    // _delay_ms(10);

    // printf("CANINTF = %02x  TXB0CTRL = %02x\n",
    //        MCP_read(REG_CANINTF), MCP_read(REG_TXB0CTRL));

    // if (MCP_read(REG_CANINTF) & 0x01) {        // RX0IF
    //     CAN_receive_message(&msg_rx);
    //     printf("id=%x len=%d d0=%x d1=%x\n",
    //            msg_rx.id, msg_rx.length, msg_rx.data[0], msg_rx.data[1]);
    // } else {
    //     printf("rien recu\n");
    // }
    

    
    
    CAN_send_message(msg_tx);
    _delay_ms(500);


    // Can_message msg_pos_joystick = {
    //     .id = 0x041, 
    //     .length = 2
    // };

    // while(1){
    //     Joystick joysitck = io_read_joystick();
    //     _delay_ms(20);
    //     msg_pos_joystick.data[0] = joysitck.x ; 
    //     msg_pos_joystick.data[1] = joysitck.y ;

    //     CAN_send_message(msg_pos_joystick);

    //     _delay_ms(500);
    // }

    printf("programme finit");

    return 0;
}