#include "../../include/logger.h"
#include <errno.h>

void log_puterror(int fd, char* err) {
	int no = errno;
	log_putc(fd, err);
	log_puts(fd, SPLITERAL(": "));
	log_putc(fd, strerror(no));
	log_putln(fd);
}
