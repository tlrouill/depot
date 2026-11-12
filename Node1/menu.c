#include "system_definitions.h"
#include <util/delay.h>
#include <stdio.h>

#include "menu.h"
#include "oled_driver.h"
#include "io_interface_driver.h"


uint8_t oled_run_simple_menu(Buttons btns, uint8_t goals) {
    static uint8_t current_selection = 0;
    static uint8_t needs_refresh = 1;
    static uint8_t shown_goals = 0;
    static Buttons last = {0};

    uint8_t action = MENU_NONE;

    if (goals != shown_goals) {           // nouveau but : on redessine
        shown_goals = goals;
        needs_refresh = 1;
    }

    // Une action par appui (front montant), sans _delay_ms()
    if (btns.ND && !last.ND) {
        current_selection = (current_selection + 1) % MENU_N_ITEMS;
        needs_refresh = 1;
    }
    if (btns.NU && !last.NU) {
        current_selection = (current_selection == 0) ? MENU_N_ITEMS - 1 : current_selection - 1;
        needs_refresh = 1;
    }
    if (btns.NB && !last.NB && current_selection == 1) {
        action = MENU_RESET;
    }
    last = btns;

    if (needs_refresh) {
        char ligne[17];

        oled_clear_display();
        oled_go_to_line(0);
        oled_go_to_column(0);
        oled_printf("PING-PONG");

        oled_go_to_line(2);
        oled_go_to_column(0);
        sprintf(ligne, "%c Buts : %u", current_selection == 0 ? '>' : ' ', shown_goals);
        oled_printf(ligne);

        oled_go_to_line(3);
        oled_go_to_column(0);
        oled_printf(current_selection == 1 ? "> Reset partie" : "  Reset partie");

        needs_refresh = 0;
    }

    return action;
}

void io_update_led_from_buttons(void) {
    Buttons btns;

    // 1. Lecture de l'état des boutons (n'oubliez pas le Chip Select)
    btns = io_read_buttons();

    // 2. Vérification globale
    // Si l'octet 'right', 'left' ou 'nav' n'est pas nul, au moins un bit (bouton) est à 1[cite: 121]
    if (btns.right != 0 || btns.left != 0 || btns.nav != 0) {
        // Allume la LED numéro 0 de la carte IO[cite: 121]
        io_set_led(0, 1);
    } else {
        // Éteint la LED numéro 0[cite: 121]
        io_set_led(0, 0);
    }
}