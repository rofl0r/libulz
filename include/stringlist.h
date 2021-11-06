#ifndef STRINGLIST_H
#define STRINGLIST_H

#include "sblist.h"
#define _GNU_SOURCE
#include <string.h>

#define stringlist sblist
#define stringlist_new(N) sblist_new(sizeof(char*), N)
#define stringlist_free(SL) sblist_free(SL)
#define stringlist_getsize(SL) sblist_getsize(SL)
static inline char* stringlist_get(stringlist *sl, size_t idx) {
	char** x = sblist_get(sl, idx);
	return x?*x:0;
}
#define stringlist_iter(SL, IVAR) for(IVAR=0;IVAR<sblist_getsize(SL);IVAR++)
#define __strlist_iterator_name __sblist_macro_concat(sblist_iterator, __LINE__)
#define stringlist_foreach(SL, RES) size_t __strlist_iterator_name=0; \
  for(RES=(stringlist_getsize(SL)?stringlist_get(SL, 0):(char*)0); \
      __strlist_iterator_name<stringlist_getsize(SL); \
      ++__strlist_iterator_name,RES=(stringlist_getsize(SL)?stringlist_get(SL, __strlist_iterator_name):(char*)0))
// returns 1 on success, 0 on OOM
#define stringlist_add(SL, S) sblist_add(SL, &(S))
static inline int stringlist_add_dup(stringlist *sl, char* s) {
	char *x = strdup(s);
	if(x) {
		if(!sblist_add(sl, &x)) { free(x); return 0; }
		return 1;
	}
	return 0;
}
#define stringlist_delete(SL, X) sblist_delete(SL, X)

#endif

