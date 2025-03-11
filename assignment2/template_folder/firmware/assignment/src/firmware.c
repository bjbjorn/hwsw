#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 15625000; i++);
}

int main() {
	unsigned int value = 0x00;

	while (1) {
		*ADDR = value;
		delay();
		value++;
		if (value <= 0x09) {
			value = 0x00;
		}
	}
	return 0;
}
