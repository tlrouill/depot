#include "adc_driver.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


volatile uint8_t *address_ADC = (volatile uint8_t *)0x1000;




//Initializig ADC
void init_adc(){
    //Sets PD5 as ouput
    DDRD |= (1<<DDD5);

    //Toggles OC1A to compare match
    TCCR1A |= (1<<COM1A0);

    //No prescaler and sets timer1 in CTC mode
    TCCR1B |= (1<<WGM12) | (1<<CS10);
}

//Reads ADC value
uint8_t read_ADC(int adress){
    volatile uint8_t retreived_value = address_ADC[adress];
    //printf("Adress value of adress, %d, equals to: %02X\n", adress, retreived_value);

    return retreived_value;
}

//Converts digital value to voltage
double voltage_conversion (uint8_t digital_signal){

    double voltage = ((double)digital_signal/256)*5;
    //int test = ((int)digital_signal/256)*5*1000;
    //printf("DV x: %d\n", digital_signal);
    //printf("Voltage x: %i\n", test);
    return voltage;

}

//Converts voltage to angle x in percentage
int angle_conversion (double voltage){

    double angle_x = (voltage-2.5)/0.015;
    int angle_x_int = (int)angle_x;
    return angle_x_int;
}


int master_conversion_x (uint8_t digital_signal){

    double voltage_x = voltage_conversion(digital_signal);
    int angle_x = (angle_conversion(voltage_x)-48);
    if(angle_x > 0) {
        angle_x = angle_x*100/113;
        if(angle_x>100){
            angle_x=100;
        }
    }
    if(angle_x <= 0) {
        angle_x = angle_x*100/130;
        if(angle_x<-100){
            angle_x=-100;
        }
    }


    return angle_x;
}


int master_conversion_y (uint8_t digital_signal){

    double voltage_y = voltage_conversion(digital_signal);
    int angle_y = angle_conversion(voltage_y) - 46;
    if(angle_y > 0) {
        angle_y = angle_y*100/115;
        if(angle_y>100){
            angle_y=100;
        }
    }
    if(angle_y <= 0) {
        angle_y = angle_y*100/122;
        if(angle_y<-100){
            angle_y=-100;
        }
    }

    return angle_y;
}

direction joystick_position(int posx, int posy){
    float r = sqrt(posx * posx + posy * posy);
    if((r < 5)){
        return NEUTRAL;
    }
    if((posy >= abs(posx)) && (posy >= 0)){
        return UP;
    }
    if((abs(posy) >= abs(posx)) && (posy < 0)){
        return DOWN;
    }
    if((abs(posx) > abs(posy)) && (posx < 0)){
        return LEFT;
    }
    if((posx > abs(posy)) && (posx >= 0)){
        return RIGHT;
    }
}

int voltage_conversion_slider (uint8_t digital_signal){

    double pos = ((double)digital_signal/256)*100; 
    int pos_int = (int)pos;

    return pos_int;

}
