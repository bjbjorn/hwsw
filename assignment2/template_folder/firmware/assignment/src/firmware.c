#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 6666666; i++);
}

int main() {
	unsigned int value = 0x01;

	while (1) {
		*ADDR = value;

		value = 0x01;
		*ADDR = value;
		delay();
		value = 0x02;
		*ADDR = value;
		delay();
		value = 0x03;
		*ADDR = value;
		delay();
		value = 0x04;
		*ADDR = value;
		delay();
		value = 0x05;
		*ADDR = value;
		delay();
		value = 0x06;
		*ADDR = value;
		delay();
		value = 0x07;
		*ADDR = value;
		delay();
		value = 0x08;
		*ADDR = value;
		delay();
	}
	return 0;
}
