#ifndef FORMAT_H
#define FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __GNUC__
/* format (ARCHETYPE, STRING-INDEX, FIRST-TO-CHECK)
   ARCHETYPE is printf, scanf, strftime or strfmon
   STRING-INDEX is index of format string (starting with 1)
   FIRST-TO-CHECK is index of first argument to check (-"-)
   http://ohse.de/uwe/articles/gcc-attributes.html#func-format */
#  define PRINTF_FORMAT1 __attribute__ ((format (printf, 1, 2)))
#  define PRINTF_FORMAT2 __attribute__ ((format (printf, 2, 3)))
#  define PRINTF_FORMAT3 __attribute__ ((format (printf, 3, 4)))
#else
#  define PRINTF_FORMAT1
#  define PRINTF_FORMAT2
#  define PRINTF_FORMAT3
#endif

void ulz_printf(const char* fmt, ...) PRINTF_FORMAT1;
void ulz_fprintf(int fd, const char* fmt, ...) PRINTF_FORMAT2;
ssize_t ulz_snprintf(char* dest, size_t destsize, const char* fmt, ...) PRINTF_FORMAT3;
ssize_t ulz_vsnprintf(char* dest, size_t destsize, const char* format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/format/*.c"

