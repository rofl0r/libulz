#include "../../include/logger.h"
#include "../../include/strlib.h"
#include <unistd.h>

void log_putd(int fd, uint64_t number, int signflag) {
	char buf[32];
	char* cnv = numberToString(number, signflag, 10, buf, 0, 0);
	if(cnv)
		write(fd, cnv, strlen(cnv));
}
