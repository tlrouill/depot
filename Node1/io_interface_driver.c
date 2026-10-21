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