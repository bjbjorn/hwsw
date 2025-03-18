#include "tcnt.h"
#define LED_BASEAxDDRESS 0x80000000
#define WGM_BASEAxDDRESS 0x00000001

#define LED_REG0_ADDRESS (LED_BASEAxDDRESS + 0*4)

#define LED              (*(volatile unsigned int *) LED_REG0_ADDRESS)
#define WGM              (*(volatile unsigned int *) WGM_BASEAxDDRESS)




void irq_handler(unsigned int cause) {

    if (cause & 4) {
        LED = 0xFFFFFFFF;
    }

}


void main(void) {
    
    unsigned int i=1, j;
    WGM = 4;
    TCNT_start();
    for(j=0;j<10;j++) {
        for(i=0;i<8;i++) {
            LED = i;
        }
    }
    TCNT_stop();
}
