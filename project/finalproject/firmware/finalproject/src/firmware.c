// #include <stdio.h>
#include "sensor.h"
#include "tcnt.h"

#define OUT_BASE_ADDRESS 0x80000000
#define OUT_REG0_ADDRESS (OUT_BASE_ADDRESS + 0*4)
#define OUTPUT           (*(volatile unsigned int *) OUT_REG0_ADDRESS)

extern unsigned int sw_mult(unsigned int x, unsigned int y);// Replace with your actual implementation

void irq_handler(unsigned int cause) {}

// void initialise(unsigned char r[C_WIDTH][C_HEIGHT], unsigned char g[C_WIDTH][C_HEIGHT], unsigned char b[C_WIDTH][C_HEIGHT], unsigned char a[C_WIDTH][C_HEIGHT]) {
//     unsigned char w, h;
//     for(h=0; h<4; h++) {
//         for(w=0; w<4; w++) {
//             r[h][w] = 255; g[h][w] = 0; b[h][w] = 0; a[h][w] = 255;
//         }
//         for(w=4; w<C_WIDTH; w++) {
//             r[h][w] = 0; g[h][w] = 255; b[h][w] = 0; a[h][w] = 255;
//         }
//     }
//     for(h=4; h<C_HEIGHT; h++) {
//         for(w=0; w<4; w++) {
//             r[h][w] = 0; g[h][w] = 0; b[h][w] = 255; a[h][w] = 255;
//         }
//         for(w=4; w<C_WIDTH; w++) {
//             r[h][w] = 127; g[h][w] = 127; b[h][w] = 127; a[h][w] = 255;
//         }
//     }
// }

// void initialise(unsigned char r[C_HEIGHT][C_WIDTH], unsigned char g[C_HEIGHT][C_WIDTH], unsigned char b[C_HEIGHT][C_WIDTH], unsigned char a[C_HEIGHT][C_WIDTH]) {
//     for (unsigned char h = 0; h < C_HEIGHT; h++) {
//         for (unsigned char w = 0; w < C_WIDTH; w++) {
//             unsigned int pixel = SENSOR_fetch();
//             r[h][w] = (pixel >> 24) & 0xFF;
//             g[h][w] = (pixel >> 16) & 0xFF;
//             b[h][w] = (pixel >> 8)  & 0xFF;
//             a[h][w] = (pixel >> 0)  & 0xFF;
//         }
//     }
// }

int main(void) {

    unsigned char C_WIDTH = SENSOR_get_width();
    unsigned char C_HEIGHT = SENSOR_get_height();


    unsigned char r[C_HEIGHT][C_WIDTH];
    unsigned char g[C_HEIGHT][C_WIDTH];
    unsigned char b[C_HEIGHT][C_WIDTH];
    unsigned char a[C_HEIGHT][C_WIDTH];

    /* Header */
    OUTPUT = (unsigned char)'q';
    OUTPUT = (unsigned char)'o';
    OUTPUT = (unsigned char)'i';
    OUTPUT = (unsigned char)'f';

    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = C_WIDTH;

    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = C_HEIGHT;

    OUTPUT = 0x03;
    OUTPUT = 0x00;

}
