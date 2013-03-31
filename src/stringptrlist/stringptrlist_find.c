#include "../../include/stringptrlist.h"

/* searches for what in list. 
 * return index if success, -1 on failure */
ssize_t stringptrlist_find(stringptrlist* list, stringptr* what) {
	size_t i;
	stringptr* s;
	if(!list || !what) goto err;
	for(i = 0; i < stringptrlist_getsize(list); i++) {
		s = stringptrlist_get(list, i);
		if(!s) goto err;
		if(stringptr_eq(what, s)) return i;
	}
	err:
	return -1;
}
