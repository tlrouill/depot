#pragma once
#include <stdint.h>

#define JOYSTICK_CAN_ID  0x041   // doit être égal à msg_pos_joystick.id du nœud 1

typedef enum {
    NEUTRAL, LEFT, RIGHT, UP, DOWN   // même ordre que dans adc_driver.h du nœud 1
} direction;

typedef struct {
    int8_t    x;     // -100 .. 100
    int8_t    y;     // -100 .. 100
    direction dir;
    uint8_t   btn;
} JoystickData;

// Retourne 1 si un message joystick a été reçu, 0 sinon
uint8_t joystick_receive(JoystickData *js);