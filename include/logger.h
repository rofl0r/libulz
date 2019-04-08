#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#include "stringptr.h"
#include "vario.h"

void log_putln(int fd);
void log_puts(int fd, stringptr* s);
void log_putd(int fd, uint64_t number, int signflag);

/* writes a variable number of vario varargs to fd, plus a trailing newline.
 * last element has to be NULL */
void log_put(int fd, ...);
void log_putc(int fd, const char* c);
void log_puterror(int fd, const char* err);
void log_timestamp(int fd);

#define log_perror(E) log_puterror(2, (E))
#define log_putspace(X) log_puts(X, SPL(" "))

#pragma RcB2 DEP "../src/logger/*.c"

#endif

