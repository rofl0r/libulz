#include "../include/strlib.h"
#include <stdio.h>
#include <string.h>

#define A(COND) {tests++; if(!(COND)) {ret++; puts("test " #COND  " failed");}}
#define U (unsigned char*)
#define T(X, Y) { raw2hex(buf, U X, sizeof(X) -1); A(str_equal(buf, Y)); }

int main() {
	char buf[32+1];
	int ret = 0, tests = 0;
	T("\x01\x05\x0a", "01050a");
	T("\xff\xde\xad\xc0\xde\x00\x01", "ffdeadc0de0001");
	T("\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xaa\xbb\xcc\xdd\xee\xff", "00112233445566778899aabbccddeeff");
	printf("%d out of %d tests failed.\n", ret, tests);
	return ret;
}
