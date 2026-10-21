#ifndef MCP
#define MCP

#include <stdlib.h>
#include <avr/io.h>



uint8_t MCP_read(uint8_t adress);

void MCP_write(uint8_t adress, uint8_t data );

void MCP_request_to_send(uint8_t TXB);

uint8_t MCP_read_status(void);

void MCP_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);

void MCP_reset(void);

#endif