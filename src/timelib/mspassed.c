#include "../../include/timelib.h"
#include <sys/time.h>

long mspassed(struct timeval* t) {
	struct timeval now, diff;
	gettimeofday(&now, NULL);
	timersub(&now, t, &diff);
	return (diff.tv_sec * 1000)  + (diff.tv_usec / 1000);
}
