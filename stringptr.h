/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */

#ifndef _STRINGPTR_H_
#define _STRINGPTR_H_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	char *ptr;
	size_t size;
} stringptr;


#define SPLITERAL(X) &(stringptr){(X), (sizeof((X)) - 1)}

int stringhere(stringptr* haystack, size_t bufpos, stringptr* needle);
stringptr* new_string(size_t size);
void free_string(stringptr* string);
stringptr* stringptr_grow(stringptr*s, size_t newsize);
stringptr* stringptr_concat(stringptr* self, ...);
stringptr* copy_string(stringptr* s);
size_t getfilesize(char* filename);
size_t writefile(char* filename, stringptr* buffer);
stringptr* readfile(char* filename);
stringptr* format(char* fmt, ...);
int streq(stringptr* a, stringptr* b);
int stringptr_shiftright(stringptr* s, size_t count);
int stringptr_shiftleft(stringptr* s, size_t count);
size_t stringptr_chomp(stringptr* s);
char* stringptr_strdup(stringptr* s);
char* stringptr_rchr(stringptr* haystack, int needle);

#ifdef __cplusplus
}
#endif

#endif
