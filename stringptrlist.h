#ifndef STRINGPTRLIST_H_
#define STRINGPTRLIST_H_

#include "stringptr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	size_t size;
	size_t capa;
} stringptrlist;

stringptrlist* stringptr_splitc(stringptr* buf, int delim);
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim);
stringptrlist* parselines(stringptr* buf);
stringptrlist* new_stringptrlist(size_t items);
stringptrlist* resize_stringptrlist(stringptrlist* list, size_t items);
int stringptrlist_add(stringptrlist** l, char* s, size_t len);
int setlistitem(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
stringptr* getlistitem(stringptrlist* l, size_t itemnumber);

// logically belonging to stringptr.c, but needs list to operate
stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit);

/*
stringptrlist* listdir(char* dirname);
void stringptrlist_sort(stringptrlist* l);
*/

#ifdef __cplusplus
}
#endif

#endif
