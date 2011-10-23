#ifndef TIMELIB_H
#define TIMELIB_H

#include <stddef.h>

int msleep(long millisecs);
int microsleep(long microsecs);
size_t timestamp(char* buffer, size_t bufsize);

#endif

//RcB: DEP "../src/timelib/*.c"
