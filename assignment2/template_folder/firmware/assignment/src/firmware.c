#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 125000000; i++);
}

int main() {
	unsigned int value = 0x01;

	while (1) {
		*ADDR = value;

		value = 0x01;
		delay();
		value = 0x02;
		delay();
		value = 0x03;
		delay();
		value = 0x04;
		delay();
		value = 0x05;
		delay();
		value = 0x06;
		delay();
		value = 0x07;
		delay();
		value = 0x08;
		delay();
	}
	return 0;
}
