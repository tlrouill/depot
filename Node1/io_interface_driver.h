#ifndef IO
#define IO

#include <stdlib.h>
#include <avr/io.h>


typedef struct __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
} Buttons;

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t size; 
} TouchPad;


typedef struct {
    uint8_t x;
    uint8_t size; // Force du signal du slider
} TouchSlider;


typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t btn; // État du bouton central du joystick
} Joystick;


Buttons io_read_buttons(void);

Joystick io_read_joystick(void) ; 

TouchPad io_read_touchpad(void); 

TouchSlider io_read_slider(void); 

void io_set_led(uint8_t led_n, uint8_t state);

void io_set_led_pwm(uint8_t led_n, uint8_t width);


#endif