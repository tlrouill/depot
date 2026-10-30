#include "joystick.h"
#include "can.h"

uint8_t joystick_receive(JoystickData *js) {
    CanMsg m;

    if (!can_rx(&m)) {
        return 0;                       // aucun message
    }
    if (m.id != JOYSTICK_CAN_ID || m.length < 2) {
        return 0;                       // autre message, ou trop court
    }

    js->x   = (int8_t)m.byte[0];        // relu en signé
    js->y   = (int8_t)m.byte[1];
    js->dir = (m.length > 2) ? (direction)m.byte[2] : NEUTRAL;
    js->btn = (m.length > 3) ? m.byte[3] : 0;
    return 1;
}