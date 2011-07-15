//RcB: DEP "../strlib.c"
#include "../strlib.h"
#include <assert.h>
#include <stdint.h>

int main(void) {
	int testint;
	unsigned testuint;
	int64_t testint64;
	uint64_t testuint64;
	
	char buffer[32];
	
	testint = 0xFFFFFFFF;
	testuint = 0xFFFFFFFF;
	testint64 = 0xFFFFFFFFFFFFFFFFL;
	testuint64 = 0xFFFFFFFFFFFFFFFFUL;
	
	int base = 52;
	
	puts(numberToString((int64_t) testint, 1, base, buffer, 16, 0));
	puts(numberToString((uint64_t)testuint, 0, base, buffer, 16, 0));
	puts(numberToString(testint64, 1, base, buffer, 0, 0));
	puts(numberToString(testuint64, 0, base, buffer, 0, 0));
	
	puts(numberToString((int64_t) testint, 1, base, buffer, 0, 1));
	puts(numberToString((uint64_t)testuint, 0, base, buffer, 0, 1));
	puts(numberToString(testint64, 1, base, buffer, 20, 1));
	puts(numberToString(testuint64, 0, base, buffer, 20, 1));
	
	
	return 0;
}