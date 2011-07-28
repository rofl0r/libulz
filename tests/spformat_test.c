#include <stdio.h>
#include <assert.h>
#include "../include/strlib.h"

int main() {
	char buf[128];
	
	ssize_t ret;
	
	ret = ulz_snprintf(buf, sizeof(buf), "[%.4d] foo bar %s\n", 13, "baz");
	assert(ret == 19);
	puts(buf);
	ret = ulz_snprintf(NULL, 0, "[%.4d] foo bar %s\n", 13, "baz");
	assert(ret == 19);

	ret = ulz_snprintf(buf, sizeof(buf), "[%d] foo bar %s%%%zu,%lld\n", 13, "baz", (size_t) 0xFFFFFFFFFFFFFFFFL, (long long) 0xFFFFFFFFFFFFFFFFL);
	puts(buf);
	assert(ret == snprintf(buf, sizeof(buf), "[%d] foo bar %s%%%zu,%lld\n", 13, "baz", (size_t) 0xFFFFFFFFFFFFFFFFL, (long long) 0xFFFFFFFFFFFFFFFFL));
	//puts(buf);

	ret = ulz_snprintf(buf, sizeof(buf), "[%d] foo bar %s%%%zu,%lld\n%", 13, "baz", (size_t) 0xFFFFFFFFFFFFFFFFL, (long long) 0xFFFFFFFFFFFFFFFFL);
	assert(ret == -1);
	
	ret = ulz_snprintf(NULL, 0, "[%.4d] foo bar %s\n", 13, "baz");
	assert(ret == 19);
	
	ret = ulz_snprintf(buf, 4, "%d", 1337);
	assert(ret == 3);
	puts(buf);
	
	ret = ulz_snprintf(NULL, 0, "%d", 1337);
	assert(ret == 4);
	
	ret = ulz_snprintf(buf, sizeof(buf), "%p", buf);
	puts(buf);
	
	
	return 0;
}
