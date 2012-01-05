#include "../../include/strlib.h"
#include "../../include/filelib.h"

/* calls mkdtemp on /dev/shm and on failure on /tmp, to get the fastest possible
 * storage. returns size of the string returned in buffer */
size_t mktempdir(char* prefix, char* buffer, size_t bufsize) {
	ssize_t ret;
	ret = ulz_snprintf(buffer, bufsize, "/dev/shm/%sXXXXXX", prefix);
	if(!ulz_mkdtemp(buffer)) {
		ret = ulz_snprintf(buffer, bufsize, "/tmp/%sXXXXXX", prefix);
		if(!ulz_mkdtemp(buffer)) return 0;
	}
	return ret;
}
