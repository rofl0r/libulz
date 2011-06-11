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

stringptr* new_string(size_t size);
void free_string(stringptr* string);
stringptr* copy_string(stringptr* s);
size_t getfilesize(char* filename);
size_t writefile(char* filename, stringptr* buffer);
stringptr* readfile(char* filename);
stringptr* format(char* fmt, ...);
int streq(stringptr* a, stringptr* b);
stringptrlist* parselines(stringptr* buf);
stringptrlist* new_stringptrlist(size_t items);
stringptrlist* resize_stringptrlist(stringptrlist* list, size_t items);
void setlistitem(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
stringptr* getlistitem(stringptrlist* l, size_t itemnumber);

#ifdef __cplusplus
}
#endif

#endif
