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

// parses line of a textfile.
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, '\n' in the original buffer will be replaced with '\0'
#define stringptr_linesplit(b) stringptr_splitc(b, '\n')


stringptrlist* stringptr_splitc(stringptr* buf, int delim);
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim);
stringptrlist* stringptrlist_new(size_t items);
stringptrlist* stringptrlist_resize(stringptrlist* list, size_t items);
int stringptrlist_add(stringptrlist** l, char* s, size_t len);
int stringptrlist_set(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
stringptr* stringptrlist_get(stringptrlist* l, size_t itemnumber);

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

//RcB: DEP "../src/stringptrlist/*.c"

