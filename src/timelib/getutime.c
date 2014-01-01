#include "../../include/timelib.h"

long long getutime(void) {
	struct timeval t;
	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000LL * 1000LL) + t.tv_usec;
}
