// #include <stdio.h>
#include "sensor.h"
#include "tcnt.h"

#define OUT_BASE_ADDRESS 0x80000000
#define OUT_REG0_ADDRESS (OUT_BASE_ADDRESS + 0*4)
#define OUTPUT           (*(volatile unsigned int *) OUT_REG0_ADDRESS)

void irq_handler(unsigned int cause) {
    TCNT_CR = 0x17;
    TCNT_CR = 0x3;
}

int main(void) {
    TCNT_CR = 0x3;
    TCNT_start();

    unsigned char C_WIDTH = SENSOR_get_width();
    unsigned char C_HEIGHT = SENSOR_get_height();
    
    unsigned char r_prev = 0;
    unsigned char g_prev = 0;
    unsigned char b_prev = 0;
    unsigned char a_prev = 255;
    
    signed char dr;
    signed char dg;
    signed char db;
    
    signed char rle = -1;
    unsigned int running_array[64];

    for(unsigned char i = 0; i < 64; i++) {
        running_array[i] = 0;
    }

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

    for(unsigned char h = 0; h < C_HEIGHT; h++) {
        for(unsigned char w = 0; w < C_WIDTH; w++) {

            unsigned int pixel, extra_data = SENSOR_fetch();
            unsigned char r = (pixel >> 24) & 0xFF;
            unsigned char g = (pixel >> 16) & 0xFF;
            unsigned char b = (pixel >> 8)  & 0xFF;
            unsigned char a = (pixel >> 0)  & 0xFF;

            if (r == r_prev && g == g_prev && b == b_prev && a == a_prev) {
                if (rle == -1) rle = 1;
                else rle++;

                if (rle == 62) {
                    OUTPUT = (0b11000000 | (rle - 1));
                    rle = -1;
                }
                continue;
            }

            if (rle > 0) {
                OUTPUT = (0b11000000 | (rle - 1));
                rle = -1;
            }

            unsigned char index_pos = ((r << 1) + r         // r * 3
                                    + (g << 2) + g         // g * 5
                                    + (b << 3) - b         // b * 7
                                    + (a << 3) + (a << 1) + a) // a * 11
                                    & 63;
            // unsigned int packed = ((unsigned int)r << 24) | ((unsigned int)g << 16) | ((unsigned int)b << 8)  | (unsigned int)a;

            if (running_array[index_pos] == pixel) {
                OUTPUT = index_pos;
            } else {
                running_array[index_pos] = pixel;

                dr = r - r_prev;
                dg = g - g_prev;
                db = b - b_prev;

                if (extra_data != 0x00) {
                    OUTPUT = extra_data
                    r_prev = r;
                    g_prev = g;
                    b_prev = b;
                    continue;
                }


                // if (a == a_prev &&
                //     dr >= -2 && dr <= 1 &&
                //     dg >= -2 && dg <= 1 &&
                //     db >= -2 && db <= 1) {
                //     unsigned char diff_byte = 0b01000000 |
                //         ((dr + 2) << 4) |
                //         ((dg + 2) << 2) |
                //         (db + 2);
                //     OUTPUT = diff_byte;
                //     r_prev = r;
                //     g_prev = g;
                //     b_prev = b;
                //     continue;
                // }

                

                if (a == a_prev) {
                    OUTPUT = 0xFE;
                    OUTPUT = r;
                    OUTPUT = g;
                    OUTPUT = b;
                } else {
                    OUTPUT = 0xFF;
                    OUTPUT = r;
                    OUTPUT = g;
                    OUTPUT = b;
                    OUTPUT = a;
                }
            }

            r_prev = r;
            g_prev = g;
            b_prev = b;
            a_prev = a;
        }
    }
    if (rle > 0) {
        OUTPUT = (0b11000000 | (rle - 1));
        rle = -1;
    }

    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x01;

    TCNT_stop();
    TCNT_CR = 0x00; // Stop the timer
    return 0;
}
