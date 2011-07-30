#include "../include/strlib.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int getflags(int sign, int pad, int lowerhex) {
	int res = 0;
	if(sign) res |= NTS_SIGNED_TYPE;
	if(pad) res |= NTS_PAD;
	if(lowerhex) res |= NTS_LOWERCASE_CHARS;
	return res;
}

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
	
	int base = 10;
	int pad = 0;
	int lowerhex = 0;
	
	
	puts(numberToString((int64_t) testint, base, buffer, 16, getflags(1, pad, lowerhex)));
	puts(numberToString(testint64, base, buffer, 0, getflags(1, pad, lowerhex)));
	puts(numberToString((uint64_t)testuint, base, buffer, 16, getflags(0, pad, lowerhex)));
	puts(numberToString(testuint64, base, buffer, 0, getflags(0, pad, lowerhex)));
	
	pad = 1;
	puts(numberToString((int64_t) testint, base, buffer, 0, getflags(1, pad, lowerhex)));
	puts(numberToString(testint64, base, buffer, 20, getflags(1, pad, lowerhex)));
	puts(numberToString((uint64_t)testuint, base, buffer, 0, getflags(0, pad, lowerhex)));
	puts(numberToString(testuint64, base, buffer, 20, getflags(0, pad, lowerhex)));
	
	pad = 0;
	puts(numberToString((int64_t) 0, 2, buffer, 0, getflags(1, pad, lowerhex)));
	puts(numberToString((uint64_t)0, 8, buffer, 0, getflags(0, pad, lowerhex)));
	puts(numberToString(0, 10, buffer, 20, getflags(1, pad, lowerhex)));
	puts(numberToString(0, 16, buffer, 20, getflags(0, pad, lowerhex)));

	pad = 1;
	puts(numberToString((int64_t) 0, 2, buffer, 0, getflags(1, pad, lowerhex)));
	puts(numberToString((uint64_t)0, 8, buffer, 0, getflags(0, pad, lowerhex)));
	puts(numberToString(0, 10, buffer, 20, getflags(1, pad, lowerhex)));
	puts(numberToString(0, 16, buffer, 20, getflags(0, pad, lowerhex)));

	pad = 1; lowerhex = 1;
	puts(numberToString((int64_t) 0, 2, buffer, 0, getflags(1, pad, lowerhex)));
	puts(numberToString((uint64_t)0, 8, buffer, 0, getflags(0, pad, lowerhex)));
	puts(numberToString(0xff, 16, buffer, 20, getflags(1, pad, lowerhex)));
	puts(numberToString(0xff, 16, buffer, 20, getflags(0, pad, lowerhex)));
	
	
	
	return 0;
}