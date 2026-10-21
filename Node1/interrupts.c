#include <avr/io.h>
#include <avr/interrupt.h>
#include "interrupts.h" // On inclut notre propre en-tête


volatile uint8_t flag_can_message = 0;

// Fonction pour configurer l'interruption (Front descendant sur INT0)
void init_interruptions(void) {
    DDRD &= ~(1 << PD2);
    // Configuration INT0 sur front descendant
    MCUCR |= (1 << ISC01);
    MCUCR &= ~(1 << ISC00);
    
    // Activation de l'interruption INT0
    GICR |= (1 << INT0);
    
    // Activation globale des interruptions
    sei(); 
}

// La routine d'interruption qui s'exécute quand la broche INT0 passe à 0V
ISR(INT0_vect) {
    flag_can_message = 1; // On lève le drapeau
}