#include <util/delay.h>

#include "menu.h"
#include "oled_driver.h"
#include "io_interface_driver.h"


void oled_run_simple_menu(void) {
    uint8_t current_selection = 0;
    uint8_t is_selected = 0;  // 0 = Curseur '>', 1 = Curseur 'x'
    uint8_t needs_refresh = 1; // 1 = L'écran doit être redessiné
    
    Buttons btns;

    // Définition des 7 lignes selon votre demande
    const char* menu_items[7] = {
        "menu(0)",
        "menu(1)",
        "menu(2)",
        "menu(3)",
        "menu(4)",
        "menu(5)",
        "menu(6)"
    };

    while (1) {
        // Ne redessine l'écran que si une action a eu lieu
        if (needs_refresh == 1) {
            oled_clear_display();
            
            for (uint8_t i = 0; i < 7; i++) {
                oled_go_to_line(i);
                oled_go_to_column(0);
                
                // Gestion de l'affichage du curseur
                if (i == current_selection) {
                    if (is_selected == 1) {
                        oled_printf("x "); // Le bouton a été pressé
                    } else {
                        oled_printf("> "); // Navigation normale
                    }
                } else {
                    oled_printf("  "); // Ligne non sélectionnée
                }
                
                // Affichage du texte de la ligne
                oled_printf(menu_items[i]);
            }
            needs_refresh = 0; // L'affichage est à jour
        }

        // Lecture des boutons de navigation
        btns = io_read_buttons();

        // Bouton Bas (Navigation Down)[cite: 121]
        if (btns.ND == 1) { 
            current_selection++;
            if (current_selection > 6) {
                current_selection = 0; // Retour tout en haut
            }
            is_selected = 0;   // Annuler l'état "sélectionné" (croix)
            needs_refresh = 1; // Demander un rafraîchissement visuel
            _delay_ms(500);    // Anti-rebond
        }
        
        // Bouton Haut (Navigation Up)[cite: 121]
        else if (btns.NU == 1) { 
            if (current_selection == 0) {
                current_selection = 6; // Retour tout en bas
            } else {
                current_selection--;
            }
            is_selected = 0;   // Annuler l'état "sélectionné" (croix)
            needs_refresh = 1; // Demander un rafraîchissement visuel
            _delay_ms(500);    // Anti-rebond
        }
        
        // Bouton Central (Navigation Button)[cite: 121]
        else if (btns.NB == 1) { 
            is_selected = 1;   // Remplacer le '>' par 'x'
            needs_refresh = 1; // Demander un rafraîchissement visuel
            _delay_ms(500);    // Anti-rebond
        }

        // Petite pause pour relâcher le processeur et le bus SPI
        _delay_ms(20); 
    }
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