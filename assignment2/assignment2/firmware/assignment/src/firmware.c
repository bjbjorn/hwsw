#include "print.h"
#include "tcnt.h"
#define ADDR ((volatile unsigned int *)0x80000000)

struct matrix_t{
	unsigned int a00;
	unsigned int a10;
	unsigned int a01;
	unsigned int a11;
};

extern unsigned int sw_mult(unsigned int x, unsigned int y);

void delay() {
	for (volatile unsigned int i = 0; i < 3125000; i++);
}

void matrix_mult(struct matrix_t * z, struct matrix_t * x, struct matrix_t * y) {
	z->a00 = sw_mult(x->a00, y->a00) + sw_mult(x->a10, y->a01);
	z->a10 = sw_mult(x->a00, y->a10) + sw_mult(x->a10, y->a11);
	z->a01 = sw_mult(x->a01, y->a00) + sw_mult(x->a11, y->a01);
	z->a11 = sw_mult(x->a01, y->a10) + sw_mult(x->a11, y->a11);
}

int main() {
	struct matrix_t m, n, o;
	
	m.a00 = 1; n.a00 = 5;
	m.a10 = 2; n.a10 = 6;
	m.a01 = 3; n.a01 = 7;
	m.a11 = 4; n.a11 = 8;

	TCNT_start();
	matrix_mult(&o, &m, &n);
	TCNT_stop();
	unsigned int tcnt = TCNT_VAL;
	print_dec(tcnt);


	/* unsigned int value = 0;

	while (1) {
		*ADDR = value;
		delay();
		value++;
		if (value >= 16) {
			value = 0;
		}
	} */
	return 0;
}
