#include <stdarg.h>
#include "../../include/vario.h"
#include "../../include/logger.h"

#if __GNUC__ > 3
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
void log_put(int fd, ...) {
	vario* v;
	int sign;
	uint64_t number;
	stringptr cnv, *c = &cnv;
	va_list ap;
	va_start(ap, fd);
	while(1) {
		v = va_arg(ap, vario*);
		if(!v) break;
		switch(v->type) {
			case VT_STRINGPTR:
				log_puts(fd, *((stringptr**) v->ptr));
				break;
			case VT_CHAR_STAR: case VT_CONST_CHAR_STAR:
				c->ptr = *((char**) v->ptr);
				if(!c->ptr) break;
				c->size = strlen(c->ptr);
				log_puts(fd, c);
				break;
			case VT_INT: case VT_UNSIGNED:
				sign = v->type == VT_INT;
				number = sign ? (int64_t)(*((int*) v->ptr)) : (uint64_t)(*((unsigned*) v->ptr));
				goto putd;
			case VT_LONG: case VT_ULONG:
				sign = v->type == VT_LONG;
				number = sign ? (int64_t)(*((long*) v->ptr)) : (uint64_t)(*((unsigned long*) v->ptr));
				putd:
				log_putd(fd, number, sign);
				break;
			default:
				break;
		}
	}
	va_end(ap);
	log_putln(fd);
}
#if __GNUC__ > 3
#pragma GCC diagnostic warning "-Wsign-compare"
#endif

