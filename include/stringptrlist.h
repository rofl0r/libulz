#ifndef STRINGPTRLIST_H_
#define STRINGPTRLIST_H_

#include "stringptr.h"
#include "sblist.h"
#include <unistd.h>
// ^ ssize_t
#ifdef __cplusplus
extern "C" {
#endif

#define stringptrlist sblist
#define stringptrlist_new(X) sblist_new(sizeof(stringptr), (X))
#define stringptrlist_free(X) sblist_free(X)

/* the reason _add takes a ptr, len tuple is that it is often needed/more efficient
 * to have the string separately allocated.
 * also, stringptr_new returns a single allocated
 * memory buffer, where the ptr member points to the buffer past the struct header.
 * so we'd need a separate free method which recalculates the original stringptr, since
 * sblist_add dereferences the stringptr and stores only its contents. and that is quite
 * nasty and inviting bugs. */
#define stringptrlist_add(list, charptr, len) sblist_add((list), SPMAKE((charptr), (len)))
int stringptrlist_add_strdup(stringptrlist* l, stringptr* s);

#define stringptrlist_set(list, itemnumber, charptr, len) \
	sblist_set((list), SPMAKE((charptr), (len)), (itemnumber))
  
#define stringptrlist_get(list, itemnumber) \
	((stringptr*) (sblist_get((list), (itemnumber))))
	
#define stringptrlist_getsize(X) sblist_getsize(X)
#define stringptrlist_empty(X) sblist_empty(X)

/* parses lines of a stringptr.
 * it returns a stringptrlist. contents will point into the orginal buffer.
 * that means that the stringptr has to stay alive as long as the list.
 * and that the stringptrs in the list dont have to be freed.
 * 
 * also, '\n' in the original buffer will be replaced with '\0' */
#define stringptr_linesplit(b) stringptr_splitc(b, '\n')

stringptrlist* stringptr_splitc(stringptr* buf, int delim);
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim);

// logically belonging to stringptr.c, but needs list to operate
stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit);

ssize_t stringptrlist_find(stringptrlist* list, stringptr* what);
int stringptrlist_contains(stringptrlist* list, stringptr* what);
stringptr* stringptrlist_tostring(stringptrlist* l);
stringptr* stringptrlist_tostring_dos(stringptrlist* l);
int stringptrlist_tofile(stringptrlist* l, char* filename, int mode);
int stringptrlist_fromfile(stringptrlist* l, const char* fn);

/* freestrings frees the char* ptr's contained in a list.
 * note that it doesn't free a stringptr allocated via stringptr_new.
 * calling this on such an allocated stringptr would crash when 
 * calling free(). */
void stringptrlist_freestrings(stringptrlist* l);
/* calls stringptrlist_freestrings, then frees the list itself. */
void stringptrlist_freeall(stringptrlist* l);

/* duplicates all stringptr's in the list. returns -1 on success, or the index of the failed item on failure.
 all items up to this point are dup'ed. */
ssize_t stringptrlist_dup_entries(stringptrlist* l);
/*
stringptrlist* listdir(char* dirname);
void stringptrlist_sort(stringptrlist* l);
*/

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/stringptrlist/*.c"
//RcB: CFLAGS "-std=c99"
