#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <time.h>
#include "../../include/format.h"

size_t timestamp(char* buffer, size_t bufsize) {
	time_t secs = time(NULL);
	struct tm tim;
	localtime_r(&secs, &tim);
	return ulz_snprintf(buffer, bufsize, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", 1900 + tim.tm_year, tim.tm_mon + 1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);
}
