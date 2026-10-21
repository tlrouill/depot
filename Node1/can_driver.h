#ifndef CAN
#define CAN

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

// --- Configuration globale et Statut ---
#define REG_CANSTAT    0x0E  // Registre de statut du CAN
#define REG_CANCTRL    0x0F  // Registre de contrôle du CAN
#define REG_CNF3       0x28  // Configuration du Baud Rate 3
#define REG_CNF2       0x29  // Configuration du Baud Rate 2
#define REG_CNF1       0x2A  // Configuration du Baud Rate 1

// --- Gestion des Interruptions ---
#define REG_CANINTE    0x2B  // Activation des interruptions
#define REG_CANINTF    0x2C  // Drapeaux (Flags) d'interruptions

// --- Tampon de Transmission 0 (TXB0) ---
#define REG_TXB0CTRL   0x30  // Contrôle du tampon TXB0
#define REG_TXB0SIDH   0x31  // Identifiant standard (bits forts) - TXB0
#define REG_TXB0SIDL   0x32  // Identifiant standard (bits faibles) - TXB0
#define REG_TXB0DLC    0x35  // Longueur des données (Data Length Code) - TXB0
#define REG_TXB0D0     0x36  // Octet de donnée 0 - TXB0
// Note : Les registres de données suivants (D1 à D7) vont de 0x37 à 0x3D

// --- Tampon de Réception 0 (RXB0) ---
#define REG_RXB0CTRL   0x60  // Contrôle du tampon RXB0
#define REG_RXB0SIDH   0x61  // Identifiant standard (bits forts) - RXB0
#define REG_RXB0SIDL   0x62  // Identifiant standard (bits faibles) - RXB0
#define REG_RXB0DLC    0x65  // Longueur des données (Data Length Code) - RXB0
#define REG_RXB0D0     0x66  // Octet de donnée 0 - RXB0

typedef struct {
    uint16_t id;
    uint8_t length;
    uint8_t data[8]; 
} Can_message ;

void CAN_init(void); 

void CAN_send_message(Can_message message);

void CAN_receive_message(Can_message *message);


#endif