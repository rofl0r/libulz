#include <stdarg.h>
#include <unistd.h>
#include "../../include/format.h"

PRINTF_FORMAT2
void ulz_fprintf(int fd, const char* fmt, ...) {
	char dest[4096];
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = ulz_vsnprintf(dest, sizeof(dest), fmt, ap);
	va_end(ap);
	if(result) {
		write(fd, dest, result);
	}
}
