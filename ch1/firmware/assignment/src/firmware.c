#include "print.h"

unsigned int convert(unsigned int x);

int main(void) {
	unsigned int temp;

	print_str("Hello world\n");
	print_dec(457);
	temp = convert(68);
	print_dec(temp);
}

unsigned int convert(unsigned int x) {
	unsigned int result, temp, temp2;
	temp = x - 32;
	if (temp < 0) {
		temp = 0;
	}
	temp2 = (temp << 2) + temp;
	result = divide(temp2, 9);
	// print_dec(result);
	return result;
}
