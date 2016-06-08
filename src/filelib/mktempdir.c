#include "../../include/strlib.h"
#include "../../include/filelib.h"
static size_t gen_fn(char* buf, const char* prefix, size_t pl, const char* tmpdir) {
	size_t tl = strlen(tmpdir);
	size_t a = 0;
	memcpy(buf+a, tmpdir, tl);
	a+=tl;
	memcpy(buf+a,prefix,pl);
	a+=pl;
	memcpy(buf+a,"XXXXXX", 7);
	return a+6;
}

/* calls mkdtemp on /dev/shm and on failure on /tmp, to get the fastest possible
 * storage. returns size of the string returned in buffer */
size_t mktempdir(const char* prefix, char* buffer, size_t bufsize) {
	size_t ret, pl = strlen(prefix);
	if(bufsize < sizeof("/dev/shm/") -1 + pl + sizeof("XXXXXX")) return 0;
	ret = gen_fn(buffer, prefix, pl, "/dev/shm/");
	if(!ulz_mkdtemp(buffer)) {
		ret = gen_fn(buffer, prefix, pl, "/tmp/");
		if(!ulz_mkdtemp(buffer)) return 0;
	}
	return ret;
}
#ifdef TEST_MKTEMPDIR
#include <stdio.h>
int main() {
	char foo[30] = {0};
	size_t ret = mktempdir("mktempdir_test", foo, sizeof foo);
	printf("%s %d\n", foo, (int) ret);
	return !(ret == strlen(foo));
}
#endif
