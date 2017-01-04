#include "../../include/strlib.h"
#include <stdlib.h>

void base64enc(char *dst, unsigned const char* src, size_t count) {
	//if(!dst_len || (dst_len-1)/4 < (count+2)/3) return 0;
	unsigned const char *s = src;
	char* d = dst;
	while(count) {
		int i = 0,  n = *s << 16;
		s++;
		count--;
		if(count) {
			n |= *s << 8;
			s++;
			count--;
			i++;
		}
		if(count) {
			n |= *s;
			s++;
			count--;
			i++;
		}
		*d++ = base64_tbl[(n >> 18) & 0x3f];
		*d++ = base64_tbl[(n >> 12) & 0x3f];
		*d++ = i ? base64_tbl[(n >> 6) & 0x3f] : '=';
		*d++ = i == 2 ? base64_tbl[n & 0x3f] : '=';
	}
	*d = 0;
}
