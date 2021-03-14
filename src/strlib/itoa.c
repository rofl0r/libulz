#include "../../include/strlib.h"

/*
  convert int to string. buffer must be big enough to hold
  the maximum value + 1 byte for leading minus + 1 byte for
  zero termination, i.e. strlen("4294967295") + 2 == 12.

  this function deliberately doesn't use pointer arithmetic
  because it was written for agscc originally. */

char *itoa(int number, char* buffer) {
	int lentest, len = 0, i, start;

	if(number >> 31) {
		number = number * -1;
		buffer[0] = '-';
		start = 1;
	} else
		start = 0;

	lentest = number;
	do {
		len++;
		lentest /= 10;
	} while(lentest);
	buffer[start+len] = 0;
	do {
		i = number % 10;
		buffer[start+len - 1] = '0' + i;
		number -= i;
		len -= 1;
		number /= 10;
	} while (number);
	return buffer;
}

#ifdef TEST
#include <stdio.h>
int main() {
	char buf[128];
	itoa(-1337, buf);
	puts(buf);
	itoa(0, buf);
	puts(buf);
	itoa(100000, buf);
	puts(buf);
	itoa(-5653452, buf);
	puts(buf);
	return 0;
}
#endif

