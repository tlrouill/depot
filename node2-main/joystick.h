#pragma once
#include <stdint.h>
#include "can.h"      

#define JOYSTICK_CAN_ID  0x041   

typedef enum {
    NEUTRAL, LEFT, RIGHT, UP, DOWN   
} direction;

typedef struct {
    int8_t    x;     // -100 .. 100
    int8_t    y;     // -100 .. 100
    direction dir;
    uint8_t   btn;
} JoystickData;


uint8_t joystick_decode(CanMsg *m, JoystickData *js);