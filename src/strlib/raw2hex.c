#include "../../include/strlib.h"
#include <stddef.h>

void raw2hex(char *dst, const unsigned char *src, size_t len) {
	size_t i, o;
	static const char hextab[] = "0123456789abcdef";
	for (i = 0, o=0; i < len; i++, o+=2) {
		dst[o + 0] = hextab[0xf & (src[i] >> 4)];
		dst[o + 1] = hextab[0xf & src[i]];
	}
	dst[o] = 0;
}

