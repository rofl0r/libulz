#include "../../include/logger.h"
#include <errno.h>

void log_perror(char* err) {
	int no = errno;
	log_putc(2, err);
	log_puts(2, SPLITERAL(": "));
	log_putc(2, strerror(no));
	log_putln(2);
}
