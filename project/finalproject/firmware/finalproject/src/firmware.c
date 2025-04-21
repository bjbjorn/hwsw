#include "sensor.h"
#include "tcnt.h"

#define C_WIDTH 8
#define C_HEIGHT 8

#define OUT_BASEAxDDRESS 0x80000000
#define OUT_REG0_ADDRESS (OUT_BASEAxDDRESS + 0*4)
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
    // printf("Starting...\n");
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
    // printf("Initialised arrays\n");

    /* Header */
    OUTPUT = 'q';
    // printf("OUTPUT = %c\n", 'q');
    OUTPUT = 'o';
    // printf("OUTPUT = %c\n", 'o');
    OUTPUT = 'i';
    // printf("OUTPUT = %c\n", 'i');
    OUTPUT = 'f';
    // printf("OUTPUT = %c\n", 'f');

    OUTPUT = 0x00; // Width MSB
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x00;
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x00;
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x08; // Width LSB = 8
    // printf("OUTPUT = 0x%X\n", 0x0);

    OUTPUT = 0x00; // Height MSB
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x00;
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x00;
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x08; // Height LSB = 8
    // printf("OUTPUT = 0x%X\n", 0x0);

    OUTPUT = 0x04; // Channels: 4 (RGBA)
    // printf("OUTPUT = 0x%X\n", 0x0);
    OUTPUT = 0x00; // Colours: 0 = sRGB
    // printf("OUTPUT = 0x%X\n", 0x0);
    
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
                    // printf("OUTPUT = 0x%X\n", (0b11000000 | (rle - 1)));
                    rle = -1;
                }
                continue;
            }
            
            if (rle > 0) {
                OUTPUT = (0b11000000 | (rle - 1));
                // printf("OUTPUT = 0x%X\n", (0b11000000 | (rle - 1)));
                rle = -1;
            }
            
            unsigned char index_pos = (sw_mult(cr,3) + sw_mult(cg,5) + sw_mult(cb,7)+ sw_mult(ca,11)) & 63;
            unsigned int packed = (cr << 24) | (cg << 16) | (cb << 8) | ca;
            
            if (running_array[index_pos] == packed) {
                OUTPUT = index_pos;
                // printf("OUTPUT = 0x%X\n", index_pos);
            } else {
                running_array[index_pos] = packed;
            
                if (ca == a_prev) {
                    OUTPUT = 0xFE;
                    // printf("OUTPUT = 0x%X\n", 0xFE);
                    OUTPUT = cr;
                    // printf("OUTPUT = 0x%X\n", cr);
                    OUTPUT = cg;
                    // printf("OUTPUT = 0x%X\n", cg);
                    OUTPUT = cb;
                    // printf("OUTPUT = 0x%X\n", cb);
                } else {
                    OUTPUT = 0xFF;
                    // printf("OUTPUT = 0x%X\n", 0xFF);
                    OUTPUT = cr;
                    // printf("OUTPUT = 0x%X\n", cr);
                    OUTPUT = cg;
                    // printf("OUTPUT = 0x%X\n", cg);
                    OUTPUT = cb;
                    // printf("OUTPUT = 0x%X\n", cb);
                    OUTPUT = ca;
                    // printf("OUTPUT = 0x%X\n", ca);
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
