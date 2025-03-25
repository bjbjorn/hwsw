#include "tcnt.h"

#define LED_BASEAxDDRESS 0x80000000

#define LED_REG0_ADDRESS (LED_BASEAxDDRESS + 0*4)

#define LED              (*(volatile unsigned int *) LED_REG0_ADDRESS)


unsigned int leds_incr = 0;


void irq_handler(unsigned int cause) {
    leds_incr = 0x1;
    TCNT_CR = 0x17;
    TCNT_CR = 0x7;
}


void main(void) {
    unsigned int i = 1;
//    TCNT_CMP = 0xc65d40;  // for pynq board
    TCNT_CMP = 0x100;   // for simulation
    TCNT_start();
    while(1) {
        if (leds_incr) {
            i++;
            leds_incr = 0;
            if (i == 0x10) {
                i = 0;
            }
        }
        LED = i;
    }
}
