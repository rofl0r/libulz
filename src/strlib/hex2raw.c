#include <string.h>
#include "../../include/strlib.h"

static int hval(int c) {
	switch(c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
	}
	return -1;
}

size_t hex2raw(const char *hex, unsigned char* out) {
	size_t l = strlen(hex);
	int c;
	if(l&1) {
		*(out++) = hval(*hex);
		hex++;
		l++;
	}
	while(*hex) {
		c = hval(*hex);
		hex++;
		c = c << 4;
		c |= hval(*hex);
		hex++;
		*out = c;
		out++;
	}
	return l/2;
}

#ifdef TEST
#include <assert.h>
int main() {
	unsigned char buf[16];
	assert(2 == hex2raw("aab", buf));
	assert(buf[0] == 0x0a && buf[1] == 0xab);
	assert(4 == hex2raw("01020a0b", buf));
	assert(buf[0] == 0x01 && buf[1] == 0x02 && buf[2] == 0x0a && buf[3] == 0x0b);
	assert(4 == hex2raw("deadbeef", buf));
	assert(buf[0] == 0xde && buf[1] == 0xad && buf[2] == 0xbe && buf[3] == 0xef);
}
#endif
