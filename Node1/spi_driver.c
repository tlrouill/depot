#include "spi_driver.h"
#include <stdlib.h>
#include <stdio.h>

// SPI_init from lecture 7 
void SPI_init(void){
    // Set MOSI, SCK, and SS as outputs. MISO remains input.
    DDRB |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SLAVE_IO) | (1 << SPI_SLAVE_OLED) | (1 << SPI_SLAVE_CAN_CONTROLLER);
    // Set SS1, SS2, SS3  high (no slave select) initially
    PORTB |= (1 << SPI_SLAVE_IO) | (1 << SPI_SLAVE_OLED) | (1 << SPI_SLAVE_CAN_CONTROLLER);
    // SPE=1 (Enable), MSTR=1 (Master), SPR1:0=00 (F_CPU/4)
    // CPOL=0, CPHA=0 (Mode 0)
    SPCR = (1 << SPE) | (1 << MSTR);
    // SPSR: Ensure no double speed
    SPSR &= ~(1 << SPI2X);
}

// SPI_Transfer from lecture 7 
uint8_t SPI_Transfer(uint8_t byte){
    // NB: Chip must be selected prior to call
    SPDR = byte;
    while (!(SPSR & (1 << SPIF))); // Wait for transmission complete
    return SPDR;
}

uint8_t spi_read_byte(uint8_t pin){
    spi_select_slave(pin);
    uint8_t data = SPI_Transfer(0x00);
    spi_unselect_slave(pin);
    return data; 
}

void spi_write_byte(uint8_t byte, uint8_t pin){
    spi_select_slave(pin);
    SPI_Transfer(byte);
    spi_unselect_slave(pin);
}


void spi_select_slave(uint8_t pin){ 
    PORTB &= ~(1 << pin);
}

void spi_unselect_slave(uint8_t pin){
    PORTB |= (1 << pin);
}
