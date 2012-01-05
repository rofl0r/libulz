#include "../../include/timelib.h"

void gettimestamp(struct timeval* t) {
	gettimeofday(t, NULL);
}
