#include "../../include/strlib.h"
#include <string.h>
#include <stdlib.h>

int base64enc_str(char *dst, unsigned const char* src, size_t dst_len) {
	unsigned const char *s = src;
	char* d = dst;
	size_t l = dst_len;
	while(*s && l > 4) {
		int i = 0, n = *s << 16;
		s++;
		if(*s) {
			n |= *s << 8;
			s++;
			i++;
		}
		if(*s) {
			n |= *s;
			s++;
			i++;
		}
		*d++ = base64_tbl[(n >> 18) & 0x3f];
		*d++ = base64_tbl[(n >> 12) & 0x3f];
		*d++ = i ? base64_tbl[(n >> 6) & 0x3f] : '=';
		*d++ = i == 2 ? base64_tbl[n & 0x3f] : '=';
		l-=4;
	}
	if(l) {
		*d = 0;
		if(!*s) return 1;
	}
	return 0;
}
