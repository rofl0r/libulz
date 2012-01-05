#ifndef TIMELIB_H
#define TIMELIB_H

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

int msleep(long millisecs);
int microsleep(long microsecs);
size_t timestamp(char* buffer, size_t bufsize);
long mspassed(struct timeval* t);
void gettimestamp(struct timeval* t);

#endif

//RcB: DEP "../src/timelib/*.c"
