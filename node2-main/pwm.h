
#include <sam.h>
#include <stdint.h>

#define SERVO_CH        1       // PWMH1
#define SERVO_PERIOD_US 20000   // 20 ms
#define SERVO_MIN_US    900
#define SERVO_MAX_US    2100



void pwm_init(void) ;

void pwm_set_pulse_us(uint32_t us);


