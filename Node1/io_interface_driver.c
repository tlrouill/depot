#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>


#include "io_interface_driver.h"
#include "spi_driver.h"




Buttons io_read_buttons(void) {
    Buttons btn_state = {0}; 

    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x04);

    _delay_us(40);
    btn_state.right = SPI_Transfer(0x00);
    
    _delay_us(5);
    btn_state.left = SPI_Transfer(0x00);
    
    _delay_us(5);
    btn_state.nav = SPI_Transfer(0x00);

    spi_unselect_slave(SPI_SLAVE_IO);

    return btn_state;
}

Joystick io_read_joystick(void) {
    Joystick joystick_state = {0}; 

    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x03);

    _delay_us(40);
    joystick_state.x = SPI_Transfer(0x00);
    
    _delay_us(5);
    joystick_state.y = SPI_Transfer(0x00);
    
    _delay_us(5);
    joystick_state.btn = SPI_Transfer(0x00);

    spi_unselect_slave(SPI_SLAVE_IO);

    return joystick_state;
}

// Centre mesuré au repos par io_joystick_calibrate() (valeurs brutes 0..255)
static uint8_t center_x = 128;
static uint8_t center_y = 128;

// À appeler une fois au démarrage, joystick relâché
void io_joystick_calibrate(void) {
    uint16_t sum_x = 0, sum_y = 0;
    for (uint8_t i = 0; i < 8; i++) {
        Joystick j = io_read_joystick();
        sum_x += j.x;
        sum_y += j.y;
        _delay_ms(5);
    }
    center_x = sum_x / 8;
    center_y = sum_y / 8;
}

// Convertit une valeur brute en -100..100 :
//  - au-dessus du centre : échelle entre center et max
//  - en dessous du centre : échelle entre min et center
// puis limite à ±100
static int io_joystick_convert(uint8_t raw, uint8_t center, uint8_t min, uint8_t max) {
    int pos;
    if (raw >= center) {
        if (max <= center) return 0;
        pos = (int)(raw - center) * 100 / (max - center);
    } else {
        if (center <= min) return 0;
        pos = -((int)(center - raw) * 100 / (center - min));
    }
    if (pos > 100)  pos = 100;
    if (pos < -100) pos = -100;
    return pos;
}

JoystickPos io_read_joystick_pos(void) {
    Joystick raw = io_read_joystick();
    JoystickPos p;
    p.x   = io_joystick_convert(raw.x, center_x, JOY_X_MIN, JOY_X_MAX);
    p.y   = io_joystick_convert(raw.y, center_y, JOY_Y_MIN, JOY_Y_MAX);
    p.dir = joystick_position(p.x, p.y);
    p.btn = raw.btn;
    return p;
}

TouchPad io_read_touchpad(void) {
    TouchPad pad_state = {0}; 

    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x01);

    _delay_us(40);
    pad_state.x = SPI_Transfer(0x00);
    
    _delay_us(5);
    pad_state.y = SPI_Transfer(0x00);
    
    _delay_us(5);
    pad_state.size = SPI_Transfer(0x00);

    spi_unselect_slave(SPI_SLAVE_IO);

    return pad_state;
}

TouchSlider io_read_slider(void) {
    TouchSlider slider_state = {0}; 

    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x02);

    _delay_us(40);
    slider_state.x = SPI_Transfer(0x00);
    
    _delay_us(5);
    slider_state.size = SPI_Transfer(0x00);

    spi_unselect_slave(SPI_SLAVE_IO);

    return slider_state;
}

void io_set_led(uint8_t led_n, uint8_t state) {
    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x05);

    _delay_us(40);
    SPI_Transfer(led_n);
  
    _delay_us(5);
    SPI_Transfer(state);

    spi_unselect_slave(SPI_SLAVE_IO);
}

void io_set_led_pwm(uint8_t led_n, uint8_t width) {
    spi_select_slave(SPI_SLAVE_IO);
    SPI_Transfer(0x06);

    _delay_us(40);
    SPI_Transfer(led_n);
    
    _delay_us(5);
    SPI_Transfer(width);

    spi_unselect_slave(SPI_SLAVE_IO);
}