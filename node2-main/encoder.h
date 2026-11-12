#pragma once
#include <stdint.h>

void    encoder_init(void);

// Position actuelle (signée), en ticks depuis le dernier encoder_reset()
int32_t encoder_read(void);

// Remet le compteur à 0 (la position actuelle devient la référence)
void    encoder_reset(void);