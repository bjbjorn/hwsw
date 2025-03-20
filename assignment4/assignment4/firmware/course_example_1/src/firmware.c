#include "tcnt.h"

#define LED_BASEAxDDRESS 0x80000000

#define LED_REG0_ADDRESS (LED_BASEAxDDRESS + 0*4)

#define LED              (*(volatile unsigned int *) LED_REG0_ADDRESS)




void irq_handler(unsigned int cause) {

    if (cause & 4) {
        LED = 0xFFFFFFFF;
    }

}


void main(void) {
    
    unsigned int i=1, j;

    TCNT_CMP = 0xffff;
    TCNT_start();
    while(1) {
        for(i=0;i<8;i++) {
            LED = i;
        }
    }
}
