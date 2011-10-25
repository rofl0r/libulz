#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

#include "stringptr.h"
#include "vario.h"

void log_putln(int fd);
void log_puts(int fd, stringptr* s);
void log_putd(int fd, uint64_t number, int signflag);
void log_put(int fd, ...);
void log_putc(int fd, char* c);
void log_puterror(int fd, char* err);
void log_timestamp(int fd);

#define log_perror(E) log_puterror(2, (E))
#define log_putspace(X) log_puts(X, SPL(" "))

#endif

//RcB: DEP "../src/logger/*.c"

