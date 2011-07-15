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

typedef struct {
	size_t size;
} stringptrlist;

#define SPLITERAL(X) &(stringptr){X, (sizeof(X) - 1)}

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
stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit);

char* stringptr_strdup(stringptr* s);
stringptrlist* stringptr_splitc(stringptr* buf, int delim);
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim);
stringptrlist* parselines(stringptr* buf);
stringptrlist* new_stringptrlist(size_t items);
stringptrlist* resize_stringptrlist(stringptrlist* list, size_t items);
void setlistitem(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
stringptr* getlistitem(stringptrlist* l, size_t itemnumber);

#ifdef __cplusplus
}
#endif

#endif
