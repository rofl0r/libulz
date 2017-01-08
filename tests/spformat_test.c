#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/format.h"

#define ASSERTF(COND, FMT) {if(!(COND))printf("line %d: test failed. %s <%s\n", __LINE__, #COND , FMT); }
#define ASSERTI(COND, INT) {if(!(COND))printf("line %d: test failed. %s. want %d\n", __LINE__, #COND, INT); }
#define T(RET, EXP, N, FMT, ...) { \
	ret = ulz_snprintf(buf, N, FMT, __VA_ARGS__); \
	ASSERTI(ret == RET, (int)ret); \
	if(!(RET == -1)) { \
	ASSERTF(!strcmp(buf, EXP), FMT); \
	ret = snprintf(buf2, N, FMT, __VA_ARGS__); \
	ASSERTF(!strcmp(buf, buf2), FMT); \
	} }

int main() {
	char buf[128], buf2[128];

	ssize_t ret;

	T(19, "[0013] foo bar baz\n", sizeof(buf), "[%.4d] foo bar %s\n", 13, "baz");

	ret = ulz_snprintf(NULL, 0, "[%.4d] foo bar %s\n", 13, "baz");
	ASSERTF(ret == 19, "");

	T(41, "[13] foo bar baz%18446744073709551615,-1\n", sizeof(buf), "[%d] foo bar %s%%%zu,%lld\n", 13, "baz", (size_t) 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFLL);

	T(-1, "", sizeof(buf), "[%d] foo bar %s%%%zu,%lld\n%", 13, "baz", (size_t) 0xFFFFFFFFFFFFFFFFL, 0xFFFFFFFFFFFFFFFFLL);

	ret = ulz_snprintf(NULL, 0, "[%.4d] foo bar %s\n", 13, "baz");
	assert(ret == 19);

	ret = ulz_snprintf(buf, 4, "%d", 1337);
	assert(ret == 3);
	assert(!strcmp(buf, "133"));

	/* for output-buffer-too-small situations, the real snprintf differs: it does
	   zero-terminate, but returns the number of byte it would have written, would
	   the output buffer have been sufficiently big. from that derives that
	   snprintf(buf, sizeof buf, whatever) >= sizeof buf -> error. since in case of
	   error snprintf returns a negative int, when we convert if to size_t (by means
	   of sizeof buf) the negative value gets promoted to a value > INT_MAX. */
	memcpy(buf, "AAAAAA", 6+1);
	ret = snprintf(buf, 4, "%d", 1337);
	assert(ret == 4);
	assert(!memcmp(buf, "133", 4));


	ret = ulz_snprintf(NULL, 0, "%d", 1337);
	assert(ret == 4);

	ret = ulz_snprintf(buf, sizeof(buf), "%p", buf);
	//puts(buf);

	ret = ulz_snprintf(NULL, 0, "%x", 0xa);
	assert(ret == 1);

	ret = ulz_snprintf(NULL, 0, "%X", 0xa);
	assert(ret == 1);

	ret = ulz_snprintf(buf, 10, "%x", 0xa);
	assert(ret == 1);
	assert(!strcmp(buf, "a"));
	//puts(buf);

	ret = ulz_snprintf(buf, 10, "%X", 0xa);
	assert(ret == 1);
	assert(!strcmp(buf, "A"));
	//puts(buf);

	ret = ulz_snprintf(NULL, 0, "%.2x", 0xa);
	assert(ret == 2);

	ret = ulz_snprintf(NULL, 0, "%.2X", 0xa);
	assert(ret == 2);

	ret = ulz_snprintf(buf, 10, "%.2x", 0xa);
	assert(ret == 2);
	assert(!strcmp(buf, "0a"));
	//puts(buf);

	ret = ulz_snprintf(buf, 10, "%.2X", 0xa);
	assert(ret == 2);
	assert(!strcmp(buf, "0A"));
	//puts(buf);


	return 0;
}
