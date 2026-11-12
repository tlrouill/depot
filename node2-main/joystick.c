#include "joystick.h"
#include "can.h"

// Décode un message déjà lu avec can_rx() : 1 si c'était le joystick, 0 sinon
uint8_t joystick_decode(CanMsg *m, JoystickData *js) {
    if (m->id != JOYSTICK_CAN_ID || m->length < 2) {
        return 0;
    }
    js->x   = (int8_t)m->byte[0];
    js->y   = (int8_t)m->byte[1];
    js->dir = (m->length > 2) ? (direction)m->byte[2] : NEUTRAL;
    js->btn = (m->length > 3) ? m->byte[3] : 0;
    return 1;
}