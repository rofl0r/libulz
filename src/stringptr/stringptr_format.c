#include "../../include/stringptr.h"
#include "../../include/strlib.h"
#include <stdarg.h>

stringptr* stringptr_format(char* fmt, ...) {
	const size_t BUF_SIZE = 1000;
	stringptr* result = stringptr_new(BUF_SIZE);
	if(!result || !fmt) return NULL;
	va_list ap;
	va_start(ap, fmt);
	result->size = ulz_vsnprintf(result->ptr, BUF_SIZE, fmt, ap);
	va_end(ap);
	return result;
}
