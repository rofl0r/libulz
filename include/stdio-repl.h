#ifndef STDIO_REPL_H
#define STDIO_REPL_H

/* replacement for common stdio operations
 * 
 * C stdio pulls in a lot of dependencies into a static
 * linked binary. for example, anything that works using buffered
 * stdio using FILE*, pulls in the entire malloc impl.
 * likewise, anything using the vsnprintf core pulls in a lot
 * of usually unneeded stuff like floating point handling.
 * often, even simple stuff like puts() is implemented by calling
 * fprintf internally, which in turn depends on buffered IO.
 * linking all these things just to print out an error message
 * is pretty bloated.
 * 
 * so, we offer a replacement for a couple of those functions
 * that directly operate on fds (open/write API),
 * + a stripped down vsnprintf impl lacking some features,
 * but implementing the most important ones.
 *
 * WARNING: if <stdio.h> is used as well, this header must be included
 * *after* stdio.h.
 * otherwise macro replacement will affect the header and cause
 * breakage.
 * 
 * WARNING: the *printf replacement does not support %f float handling
 * also it supports width specifiers only with the %.?d syntax, where
 * ? is a placeholder for a number.
 * so be sure your format strings give the expected results before
 * shipping...
 * also checking the return value of fprintf, printf, puts, perror,
 * is not supported.
 * fprintf works only with stdout and stderr.
 * 
 */

#include "strlib.h"
#include "logger.h"

#define printf(...) ulz_printf(__VA_ARGS__)
#define snprintf(...) ulz_snprintf(__VA_ARGS__)
#define vsnprintf(...) ulz_vsnprintf(__VA_ARGS__)
#define fprintf(f, ...) ulz_fprintf(#f == "stdout" ? 1 : #f == "stderr" ? 2 : -1, __VA_ARGS__)
#define dprintf(...) ulz_fprintf(__VA_ARGS__)

#define perror(x) log_puterror(2, x)
#define puts(x) log_putc(1, x)

#endif
