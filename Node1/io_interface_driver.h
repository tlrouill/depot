#ifndef IO
#define IO

#include <stdlib.h>
#include <avr/io.h>
#include "adc_driver.h"   // type direction et joystick_position()

// Valeurs brutes extrêmes du joystick de la carte I/O.
// À mesurer : afficher io_read_joystick().x / .y en poussant à fond
// dans chaque direction, puis remplacer ces valeurs.
#define JOY_X_MIN   51     // à fond à gauche
#define JOY_X_MAX   203   // à fond à droite
#define JOY_Y_MIN   54     // à fond en bas
#define JOY_Y_MAX   204   // à fond en haut


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


// Position du joystick convertie (comme pour l'ADC)
typedef struct {
    int x;          // -100 (gauche) .. 100 (droite)
    int y;          // -100 (bas)    .. 100 (haut)
    direction dir;  // NEUTRAL, LEFT, RIGHT, UP, DOWN
    uint8_t btn;    // bouton du joystick
} JoystickPos;


Buttons io_read_buttons(void);

Joystick io_read_joystick(void) ;

void io_joystick_calibrate(void);

JoystickPos io_read_joystick_pos(void);

TouchPad io_read_touchpad(void); 

TouchSlider io_read_slider(void); 

void io_set_led(uint8_t led_n, uint8_t state);

void io_set_led_pwm(uint8_t led_n, uint8_t width);


#endif