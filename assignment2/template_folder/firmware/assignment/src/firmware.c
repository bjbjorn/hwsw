#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 6666666; i++);
}

int main() {
	unsigned int value = 0x01;

	while (1) {
		*ADDR = 0x01;
		*ADDR = 0x01;
		*ADDR = 0x02;
		delay();
		*ADDR = 0x02;
		delay();
		*ADDR = 0x03;
		delay();
		*ADDR = 0x04;
		delay();
		*ADDR = 0x05;
		delay();
		*ADDR = 0x06;
		delay();
		*ADDR = 0x07;
		delay();
		*ADDR = 0x08;
		delay();
	}
	return 0;
}
