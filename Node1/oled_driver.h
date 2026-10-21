#ifndef OLED
#define OLED

#include <stdlib.h>
#include <avr/io.h>

#define OLED_DC_PIN  PB2

void oled_init(void); 

void oled_write_data(uint8_t data);

void oled_write_command(uint8_t command); 

void oled_go_to_line(uint8_t line);

void oled_go_to_column(uint8_t column);

void oled_write_char(char ch);

void oled_clear_display(void);

void oled_printf(const char* str);



#endif