#ifndef STRINGPTRLIST_H_
#define STRINGPTRLIST_H_

#include "stringptr.h"
#include "sblist.h"

#ifdef __cplusplus
extern "C" {
#endif

#define stringptrlist sblist
#define stringptrlist_new(X) sblist_new(sizeof(stringptr), (X))
#define stringptrlist_free(X) sblist_free(X)
//int stringptrlist_add(stringptrlist** l, char* s, size_t len);
#define stringptrlist_add(X, Y, Z) sblist_add((X), SPMAKE((Y), (Z)))
//int stringptrlist_set(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
#define stringptrlist_set(W, X, Y, Z) sblist_set((W), SPMAKE((Y), (Z)), (X))
//stringptr* stringptrlist_get(stringptrlist* l, size_t itemnumber);
#define stringptrlist_get(X, Y) ((stringptr*) (sblist_get((X), (Y))))
#define stringptrlist_getsize(X) ((X)->count)
// parses line of a textfile.
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, '\n' in the original buffer will be replaced with '\0'
#define stringptr_linesplit(b) stringptr_splitc(b, '\n')



stringptrlist* stringptr_splitc(stringptr* buf, int delim);
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim);

//stringptrlist* stringptrlist_resize(stringptrlist* list, size_t items);


// logically belonging to stringptr.c, but needs list to operate
stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit);

int stringptrlist_contains(stringptrlist* list, stringptr* what);
stringptr* stringptrlist_tostring(stringptrlist* l);

/*
stringptrlist* listdir(char* dirname);
void stringptrlist_sort(stringptrlist* l);
*/

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/stringptrlist/*.c"

