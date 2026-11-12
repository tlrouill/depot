#pragma once
#include <stdint.h>



#define ADC_IR_CHANNEL   7      // A0

void     adc_init(void);
uint16_t adc_read(void);        // dernière conversion du canal IR (0..4095)
