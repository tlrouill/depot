#ifndef SPI
#define SPI

#include <stdlib.h>
#include <avr/io.h>

#define SPI_SLAVE_CAN_CONTROLLER PB0
#define SPI_SLAVE_IO PB4
#define SPI_SLAVE_OLED PB3
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7

void SPI_init(void);

uint8_t SPI_Transfer(uint8_t byte);

uint8_t spi_read_byte(uint8_t pin);

void spi_write_byte(uint8_t byte, uint8_t pin);

void spi_select_slave(uint8_t pin);

void spi_unselect_slave(uint8_t);


#endif