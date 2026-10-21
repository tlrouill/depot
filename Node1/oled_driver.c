#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


#include "fonts.h"
#include "oled_driver.h"
#include "spi_driver.h"

void oled_write_command(uint8_t command){

    PORTB &= ~(1 << OLED_DC_PIN);

    spi_write_byte(command, SPI_SLAVE_OLED);
}

void oled_write_data(uint8_t data){

    PORTB |= (1 << OLED_DC_PIN);

    spi_write_byte(data, SPI_SLAVE_OLED);
}

void oled_init(void){

    DDRB |= (1 << OLED_DC_PIN);

    oled_write_command(0xA1);
    oled_write_command(0xC8);
    oled_write_command(0xAF);

    oled_clear_display();
}

void oled_go_to_line(uint8_t line){ 
    if(line > 7){
        line = 7;
    } 
    oled_write_command(0xB0 | line); // 
}

void oled_go_to_column(uint8_t column){
    if (column > 127){
        column = 127;
    }
    // Lower nibble
    oled_write_command(0x00 | (column & 0x0F));

    // Higher nibble
    oled_write_command(0x10 | ((column >> 4) & 0x0F));

}

void oled_write_char(char c) {
    if (c < 32 || c > 127){
        c = '?';
    }

    uint8_t index = c - 32;

    // Parcourir les 8 colonnes du caractère
    for (uint8_t i = 0; i < 8; i++) {
        // Lecture de l'octet dans la mémoire PROGMEM
        uint8_t byte = pgm_read_byte(&(font8[index][i]));
        
        // Envoi à l'écran OLED
        oled_write_data(byte);
    }
}


// AI written 
void oled_clear_display(void) {
    for (uint8_t page = 0; page < 8; page++) {
        // 1. Envoyer manuellement les commandes pour se placer au début de la page
        oled_write_command(0xB0 + page); // Définit la page de 0 à 7[cite: 32]
        oled_write_command(0x00);        // Colonne 0 (4 bits de poids faible)[cite: 32]
        oled_write_command(0x10);        // Colonne 0 (4 bits de poids fort)[cite: 32]
        
        // 2. Écrire 128 fois 0x00 pour vider toute la largeur de cette page
        for (uint8_t col = 0; col < 128; col++) {
            oled_write_data(0x00);
        }
    }
}

void oled_printf(const char* str) {
    // Boucle tant qu'on n'a pas atteint la fin de la chaîne (caractère nul '\0')
    while (*str != '\0') {
        oled_write_char(*str); // Affiche le caractère pointé
        str++;                 // Avance le pointeur vers le caractère suivant
    }
}

