#ifndef MENU
#define MENU

#include <stdlib.h>
#include <avr/io.h>

#include "io_interface_driver.h"   // type Buttons

#define MENU_N_ITEMS  2    // "Buts : n" et "Reset partie"
#define MENU_NONE     0    // valeurs renvoyées par oled_run_simple_menu()
#define MENU_RESET    1


void oled_run_simple_menu(void);
void io_update_led_from_buttons(void);

#endif