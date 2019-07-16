#include "../../include/timelib.h"

long long timespec_diff(struct timespec *a, struct timespec *b) {
        return (a->tv_sec - b->tv_sec) * 1000000000
                + (a->tv_nsec - b->tv_nsec);
}

