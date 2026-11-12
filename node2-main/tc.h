#pragma once
#include <stdint.h>


typedef void (*tc_callback_t)(void);


void tc_periodic_init(uint32_t period_us, tc_callback_t cb);

void tc_stop(void);


uint32_t tc_millis(void);


void tc_delay_ms(uint32_t ms);
