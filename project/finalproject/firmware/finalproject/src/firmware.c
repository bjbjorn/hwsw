#include "sensor.h"
#include "tcnt.h"

#define C_WIDTH 8
#define C_HEIGHT 8

#define OUT_BASEADDRESS 0x80000000
#define OUT_REG0_ADDRESS (OUT_BASEADRESS + 0*4)
#define OUTPUT           (*(volatile unsigned int *) OUT_REG0_ADDRESS)


extern unsigned int sw_mult(unsigned int x, unsigned int y);

void irq_handler(unsigned int cause) {
}

void initialise(unsigned char r[C_WIDTH][C_HEIGHT], unsigned char g[C_WIDTH][C_HEIGHT], unsigned char b[C_WIDTH][C_HEIGHT], unsigned char a[C_WIDTH][C_HEIGHT]) {
    unsigned char w, h;

    for(h=0;h<4;h++) {
        for(w=0;w<4;w++) {
            r[h][w] = 255; g[h][w] = 0; b[h][w] = 0; a[h][w] = 255;
        }
        for(w=4;w<C_WIDTH;w++) {
            r[h][w] = 0; g[h][w] = 255; b[h][w] = 0; a[h][w] = 255;
        }
    }
    for(h=4;h<C_HEIGHT;h++) {
        for(w=0;w<4;w++) {
            r[h][w] = 0; g[h][w] = 0; b[h][w] = 255; a[h][w] = 255;
        }
        for(w=4;w<C_WIDTH;w++) {
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

    unsigned char dr, dg, db;

    signed char rle = -1;
    unsigned int running_array[64];
    unsigned char rv;
    unsigned char index;
    unsigned int value;

    unsigned char bit_count = 0;
    unsigned int buffer = 0;

    /* Initialisation */
    initialise(r, g, b, a);
    for(unsigned char i=0;i<64;i++) {
        running_array[i] = 0;
    }

    /* Header */
    OUTPUT = 'q';
    OUTPUT = 'o';
    OUTPUT = 'i';
    OUTPUT = 'f';

    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x08;

    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x00;
    OUTPUT = 0x08;

    OUTPUT = 0x03;
    OUTPUT = 0x00;
    
    /* Loop over pixels */
    for(unsigned char h=0;h<C_HEIGHT;h++) {
        for(unsigned char w=0;w<C_WIDTH;w++) {
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
            unsigned int packed = (cr << 24) | (cg << 16) | (cb << 8) | ca;
            
            if (running_array[index_pos] == packed) {
                OUTPUT = index_pos;
            } else {
                running_array[index_pos] = packed;
            
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

    return 0;
}
