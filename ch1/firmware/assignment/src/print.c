// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

#include "print.h"

unsigned int base_lut[] = {0,1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};

void print_chr(char ch) {
	*((volatile unsigned int*)OUTPORT) = ch;
}

void print_str(const char *p) {
	while (*p != 0)
		*((volatile unsigned int*)OUTPORT) = *(p++);
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

void print_dec(unsigned int val) {
	int leading_zero = 0;
	for (int i = 10; i > 0; i--) {
		int digit = getDigit(base_lut[i],val);
		if (digit != 0 || leading_zero != 0) {
			leading_zero = 1;
			char x="0123456789"[digit];
			print_chr(x);
		}
        for (int j = 0; j < digit; j++){
            val -= base_lut[i];
        }
	}
	print_str("\n");
}

int getDigit(int base, int number) {
    int digit = 0;
    for (int i = base;i <= number;i += base) ++digit;
    return digit;
}

void print_hex(unsigned int val, int digits) {
	unsigned int index, max;
	int i; /* !! must be signed, because of the check 'i>=0' */
	char x;

	if(digits == 0)
		return;

	max = digits << 2;

	for (i = max-4; i >= 0; i -= 4) {
		index = val >> i;
		index = index & 0xF;
		x="0123456789ABCDEF"[index];
		*((volatile unsigned int*)OUTPORT) = x;
	}
	print_str("\n");
}

unsigned int convert(unsigned int x) {
	unsigned int result, temp;
	temp = x - 32;
	temp = (temp << 2) + temp;
	result = divide(temp, 9);
	print_dec(result);
	return result;
}
