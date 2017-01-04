#include "../../include/strlib.h"
#include <string.h>
#include <stdlib.h>

static inline int chrpos(int c) {
	int i = 0;
	for(;i<64;i++) if(base64_tbl[i] == c) return i;
	return -1;
}
size_t base64dec(unsigned char* dst, const char* src, size_t dst_len) {
	const char* s = src;
	unsigned char *d = dst;
	size_t l = dst_len, o = 0;
	int n = 0, cnt = 0, skip = 0;
	if(l) for(;;) {
		int p;
		if(*s == '=') {
			skip++;
			if(skip > 2) return 0;
			p = 0;
		} else if (!*s) {
			if(cnt % 4 != 0 || !l) return 0;
			*d++ = 0;
			return o;
		} else if(skip) {
			return 0;
		} else if((p = chrpos(*s)) == -1) return 0;
		n = (n << 6) | p;
		cnt++;
		if(cnt % 4 == 0) {
			if(l < 3) return 0;
			*d++ = n >> 16;
			*d++ = n >> 8 & 0xff;
			*d++ = n & 0xff;
			l -= 3;
			o += 3-skip;
			n = 0;
		}
		s++;
	}
	return 0;
}
