#include "../../include/timelib.h"
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <errno.h>

int microsleep(long microsecs) {
	struct timespec req, rem;
	req.tv_sec = microsecs / 1000000;
	req.tv_nsec = (microsecs % 1000000) * 1000;
	int ret;
	while((ret = nanosleep(&req, &rem)) == -1 && errno == EINTR) req = rem;
	return ret;
}
