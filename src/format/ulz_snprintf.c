#include <stdarg.h>
#include "../../include/format.h"

PRINTF_FORMAT3
ssize_t ulz_snprintf(char* dest, size_t destsize, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = ulz_vsnprintf(dest, destsize, fmt, ap);
	va_end(ap);
	return result;
}
