#include "../include/md5.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>

static void md5_to_str(const unsigned char hash[16], char outbuf[32+1]) {
	size_t i;
	for (i = 0; i< 16; ++i) {
		outbuf[2 * i] = "0123456789abcdef"[15 & (hash[i] >> 4)];
		outbuf[2 * i + 1] = "0123456789abcdef"[15 & hash[i]];
	}
	outbuf[2 * i] = 0;
}

int test(unsigned char *buf, size_t len, const char expected[16*2+1]) {
	md5_ctx m;
	unsigned char out[16+1];
	char ascii[16*2+1];
	md5_init(&m);
	md5_update(&m, buf, len);
	md5_end(&m, out);
	md5_to_str(out, ascii);
	return strcmp(ascii, expected);
}

#define TESTA(A, B) assert(0 == test(A, sizeof(A)-1, B))

int main() {
	TESTA("", "d41d8cd98f00b204e9800998ecf8427e");
	TESTA("foo bar", "327b6f07435811239bc47e1544353273");
	TESTA("password", "5f4dcc3b5aa765d61d8327deb882cf99");
	return 0;
}
