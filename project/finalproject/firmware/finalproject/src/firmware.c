// #include <stdio.h>
#include "sensor.h"
#include "tcnt.h"

#define C_WIDTH 8
#define C_HEIGHT 8

#define OUT_BASE_ADDRESS 0x80000000
#define OUT_REG0_ADDRESS (OUT_BASE_ADDRESS + 0*4)
#define OUTPUT           (*(volatile unsigned int *) OUT_REG0_ADDRESS)

extern unsigned int sw_mult(unsigned int x, unsigned int y);// Replace with your actual implementation

void irq_handler(unsigned int cause) {}

void initialise(unsigned char r[C_WIDTH][C_HEIGHT], unsigned char g[C_WIDTH][C_HEIGHT], unsigned char b[C_WIDTH][C_HEIGHT], unsigned char a[C_WIDTH][C_HEIGHT]) {
    unsigned char w, h;
    for(h=0; h<4; h++) {
        for(w=0; w<4; w++) {
            r[h][w] = 255; g[h][w] = 0; b[h][w] = 0; a[h][w] = 255;
        }
        for(w=4; w<C_WIDTH; w++) {
            r[h][w] = 0; g[h][w] = 255; b[h][w] = 0; a[h][w] = 255;
        }
    }
    for(h=4; h<C_HEIGHT; h++) {
        for(w=0; w<4; w++) {
            r[h][w] = 0; g[h][w] = 0; b[h][w] = 255; a[h][w] = 255;
        }
        for(w=4; w<C_WIDTH; w++) {
            r[h][w] = 127; g[h][w] = 127; b[h][w] = 127; a[h][w] = 255;
        }
    }
}

int main(void) {
    unsigned char r[C_HEIGHT][C_WIDTH];
    unsigned char g[C_HEIGHT][C_WIDTH];
    unsigned char b[C_HEIGHT][C_WIDTH];
    unsigned char a[C_HEIGHT][C_WIDTH];

    unsigned char r_prev = 0;
    unsigned char g_prev = 0;
    unsigned char b_prev = 0;
    unsigned char a_prev = 255;

    signed char dr;
    signed char dg;
    signed char db;

    signed char rle = -1;
    unsigned int running_array[64];

    initialise(r, g, b, a);
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
            unsigned char cr = r[h][w];
            unsigned char cg = g[h][w];
            unsigned char cb = b[h][w];
            unsigned char ca = a[h][w];

            if (cr == r_prev && cg == g_prev && cb == b_prev && ca == a_prev) {
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

            unsigned char index_pos = (sw_mult(cr,3) + sw_mult(cg,5) + sw_mult(cb,7)+ sw_mult(ca,11)) & 63;
            unsigned int packed = ((unsigned int)cr << 24) | ((unsigned int)cg << 16) | ((unsigned int)cb << 8)  | (unsigned int)ca;

            if (running_array[index_pos] == packed) {
                OUTPUT = index_pos;
            } else {
                running_array[index_pos] = packed;

                dr = cr - r_prev;
                dg = cg - g_prev;
                db = cb - b_prev;

                if (ca == a_prev &&
                    dr >= -2 && dr <= 1 &&
                    dg >= -2 && dg <= 1 &&
                    db >= -2 && db <= 1) {
                    unsigned char diff_byte = 0b01000000 |
                        ((dr + 2) << 4) |
                        ((dg + 2) << 2) |
                        (db + 2);
                    OUTPUT = diff_byte;
                    r_prev = cr;
                    g_prev = cg;
                    b_prev = cb;
                    continue;
                }

                if (ca == a_prev) {
                    OUTPUT = 0xFE;
                    OUTPUT = cr;
                    OUTPUT = cg;
                    OUTPUT = cb;
                } else {
                    OUTPUT = 0xFF;
                    OUTPUT = cr;
                    OUTPUT = cg;
                    OUTPUT = cb;
                    OUTPUT = ca;
                }
            }

            r_prev = cr;
            g_prev = cg;
            b_prev = cb;
            a_prev = ca;
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

    return 0;
}
