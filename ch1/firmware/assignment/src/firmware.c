#include "print.h"

unsigned int convert(unsigned int x);

int main(void) {
	unsigned int temp;

	print_str("Hello world\n");
	print_dec(457);
	temp = convert(68);
	print_dec(temp);
}

int divide(int dividend, int divisor) {
    if (divisor == 0) {
        return -1;
    }

    int quotient = 0;
    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;

    unsigned int a = (dividend < 0) ? -dividend : dividend;
    unsigned int b = (divisor < 0) ? -divisor : divisor;

    while (a >= b) {
        unsigned int temp = b, multiple = 1;
        while (a >= (temp << 1) && (temp << 1) > temp) {
            temp <<= 1;
            multiple <<= 1;
        }
        a -= temp;
        quotient += multiple;
    }
    return sign * quotient;
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
