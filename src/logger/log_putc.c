#include "../../include/logger.h"

void log_putc(int fd, const char* c) {
	if(!c) return;
	stringptr ss, *s = &ss;
	s->ptr = (char*) c;
	s->size = strlen(c);
	log_puts(fd, s);
}
