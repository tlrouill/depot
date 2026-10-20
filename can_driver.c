#include "system_definitions.h"   // doit être le 1er include (définit F_CPU)
#include <util/delay.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "MCP2515_driver.h"
#include "can_driver.h"

void CAN_init(void){
    MCP_reset();
    _delay_ms(10);

    // baud rate configuration for 38.4 kbps :
    MCP_write(REG_CNF1, 0x0F); 
    MCP_write(REG_CNF2, 0xB1);
    MCP_write(REG_CNF3, 0x05);
    
    MCP_write(REG_RXB0CTRL, 0x60);   // pas de filtrage
    
    MCP_write(REG_CANINTE, 0x01);

    MCP_bit_modify(REG_CANCTRL, 0xE0, 0x50);
}

void CAN_send_message(Can_message message){
   while (MCP_read(REG_TXB0CTRL) & 0x08) {} // ameliorer en cherchant le registre libre entre TBX0, TBX1, TBX2

    //Chargement de lidentifiant (ID standard 11 bits)
    MCP_write(REG_TXB0SIDH, (uint8_t)(message.id >> 3));
    MCP_write(REG_TXB0SIDL, (uint8_t)(message.id << 5));

    //Définition de la longueur
    MCP_write(REG_TXB0DLC, message.length & 0x0F); // On masque avec 0x0F pour s'assurer que la taille ne dépasse pas 4 bits

    //Chargement des octets de données
    for (uint8_t i = 0; i < message.length; i++) {
        MCP_write(REG_TXB0D0 + i, message.data[i]);
    }

    MCP_request_to_send(0);
}

void CAN_receive_message(Can_message *message){
    message->id = (MCP_read(REG_RXB0SIDH) << 3) | (MCP_read(REG_RXB0SIDL) >> 5); 

    message->length = MCP_read(REG_RXB0DLC) & 0x0F; 

    for (uint8_t i = 0; i < message->length; i++) {
        message->data[i] = MCP_read(REG_RXB0D0 + i);
    }

    MCP_bit_modify(REG_CANINTF, 0x01, 0x00); // desactiver interrupt 
}