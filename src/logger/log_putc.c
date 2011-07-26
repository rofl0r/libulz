#include "../../include/logger.h"

void log_putc(int fd, char* c) {
	if(!c) return;
	stringptr ss, *s = &ss;
	s->ptr = c;
	s->size = strlen(c);
	log_puts(fd, s);
}
