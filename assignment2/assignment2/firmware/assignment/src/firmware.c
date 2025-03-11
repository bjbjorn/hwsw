#include "print.h"
#define ADDR ((volatile unsigned int *)0x80000000)

void delay() {
	for (volatile unsigned int i = 0; i < 3125000; i++);
}

int main() {
	unsigned int value = 0;

	while (1) {
		*ADDR = value;
		delay();
		value++;
		if (value >= 16) {
			value = 0;
		}
	}
	return 0;
}
