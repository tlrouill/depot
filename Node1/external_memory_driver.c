#include "external_memory_driver.h" 


void external_memory_init(){

    MCUCR |= (1<<SRE); //Enables external memory
    SFIOR &= ~(111 << XMM0);

    SFIOR |= (1<<XMM2); //Releases port pins PC7-PC4 from the SRAM
    //EMCUCR|= (1<<SRW01);
    
}