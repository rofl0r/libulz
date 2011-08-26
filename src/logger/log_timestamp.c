#include "../../include/timelib.h"
#include "../../include/logger.h"

void log_timestamp(int fd) {
	char buffer[32];
	stringptr x;
	x.ptr = buffer;
	x.size = timestamp(buffer, sizeof(buffer));
	log_puts(fd, &x);
}
