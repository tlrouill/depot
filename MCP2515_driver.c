#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "spi_driver.h"


uint8_t MCP_read(uint8_t adress){
    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 

    SPI_Transfer(0X03);
    SPI_Transfer(adress);
    uint8_t data = SPI_Transfer(0x00);

    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);

    return data; 
}

void MCP_write(uint8_t adress, uint8_t data ){
    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 

    SPI_Transfer(0X02);
    SPI_Transfer(adress);
    SPI_Transfer(data);

    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);
}

void MCP_request_to_send(uint8_t TXB){
    
    uint8_t instruction = 0x80 | (1 << TXB); //to vchoose which TXBx we are writing on

    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 
    SPI_Transfer(instruction);
    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);
}

uint8_t MCP_read_status(void){
    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 

    SPI_Transfer(0XA0);
    uint8_t status = SPI_Transfer(0x00);
    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);

    return status; 
}

void MCP_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 

    SPI_Transfer(0X05);
    SPI_Transfer(adress);
    SPI_Transfer(mask);
    SPI_Transfer(data);

    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);
} 

void MCP_reset(void){
    spi_select_slave(SPI_SLAVE_CAN_CONTROLLER); 
    SPI_Transfer(0XC0);
    spi_unselect_slave(SPI_SLAVE_CAN_CONTROLLER);
}



