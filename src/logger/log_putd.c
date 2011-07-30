#include "../../include/logger.h"
#include "../../include/strlib.h"
#include <unistd.h>

void log_putd(int fd, uint64_t number, int signflag) {
	char buf[32];
	char* cnv = numberToString(number, 10, buf, 0, signflag ? NTS_SIGNED_TYPE : 0);
	if(cnv)
		write(fd, cnv, strlen(cnv));
}
