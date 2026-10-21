#ifndef ADC
#define ADC

#include <stdlib.h>
#include <avr/io.h>

typedef enum{
    NEUTRAL,
    LEFT,
    RIGHT,
    UP,
    DOWN,
}direction;

extern volatile uint8_t *address_ADC;

void init_adc();

uint8_t read_ADC();

double voltage_conversion (uint8_t digital_signal);

int angle_conversion (double voltage);

double angle_conversion_y (double voltage);

int master_conversion_x (uint8_t digital_signal);

int master_conversion_y (uint8_t digital_signal);

direction joystick_position(int posx, int posy);

int voltage_conversion_slider (uint8_t digital_signal);

#endif