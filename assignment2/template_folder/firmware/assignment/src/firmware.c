#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 125000000; i++);
}

int main() {
	unsigned int value = 0x01;

	while (1) {
		*ADDR = value;

		value ^= 0x01;
		delay();
	}
	return 0;
}
