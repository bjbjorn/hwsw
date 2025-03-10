#include "print.h"

unsigned int convert(unsigned int x);

int main(void) {
	unsigned int counter = 0;
	unsigned int leds = 0;
	counter++;
	if (counter == 125000000) {
		counter = 0;
		leds++;
		if (leds > 8) {
			leds = 0;
		}
		*((volatile unsigned int*)OUTPORT) = leds;
	}
}
