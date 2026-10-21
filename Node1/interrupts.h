#ifndef INTERRUPTS
#define INTERRUPTS

#include <stdint.h>

// On prévient les autres fichiers que cette variable existe ailleurs.
// On ne l'initialise PAS ici (pas de "= 0").
extern volatile uint8_t flag_can_message;

// Prototype de la fonction d'initialisation
void init_interruptions(void);

#endif