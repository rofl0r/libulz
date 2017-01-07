#include "../include/md5.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>

int test(unsigned char *buf, size_t len, const char expected[16*2+1]) {
	char ascii[16*2+1];
	md5str(ascii, buf, len);
	return strcmp(ascii, expected);
}

#define TESTA(A, B) assert(0 == test((unsigned char*) A, sizeof(A)-1, B))

int main() {
	TESTA("", "d41d8cd98f00b204e9800998ecf8427e");
	TESTA("foo bar", "327b6f07435811239bc47e1544353273");
	TESTA("password", "5f4dcc3b5aa765d61d8327deb882cf99");
	return 0;
}
