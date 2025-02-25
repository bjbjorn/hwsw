#include "print.h"

unsigned int convert(unsigned int x);

int main(void) {
	unsigned int temp;

	// print_str("Hello world\n");
	// print_dec(457);
	temp = convert(87);
	print_dec(temp);
}


int divide(int dividend, int divisor) {
    if (divisor == 0) {
		// print_str("nul division\n");
        return -1;
    }

    int quotient = 0;
	// print_str("sign check\n");
    int sign = ((dividend < 0) ^ (divisor < 0)) ? -1 : 1;

    unsigned int a = (dividend < 0) ? -dividend : dividend;
    unsigned int b = (divisor < 0) ? -divisor : divisor;

    while (a >= b) {
        unsigned int temp = b;
		unsigned int multiple = 1;
        while (a >= (temp << 1) && (temp << 1) > temp) {
			// print_str("shift, divisorTemp = ");
			// print_dec(temp);
            temp <<= 1;
            multiple <<= 1;
        }
        a -= temp;
        quotient += multiple;
    }
	// print_str("done\n");
	// print_dec(quotient);
    return quotient;
}

unsigned int convert(unsigned int x) {
	unsigned int result, temp, temp2;
	// print_str("Converting\n");
	temp = x - 32;
	// print_str("temp: ");
	// print_dec(temp);
	if (temp < 0) {
		temp = 0;
	}
	// print_str("temp nul check: ");
	// print_dec(temp);
	temp2 = (temp << 2) + temp;
	// print_str("temp2: ");
	// print_dec(temp2);
	result = divide(temp2, 9);
	// print_str("result: ");
	// print_dec(result);
	// print_dec(result);
	return result;
}
