#include <unistd.h>
#include "../../include/logger.h"

void log_puts(int fd, stringptr* s) {
	if(!s || !s->size) return;
	write(fd, s->ptr, s->size);
}
