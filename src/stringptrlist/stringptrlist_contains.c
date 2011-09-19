#include "../../include/stringptrlist.h"

// return 1 if success
int stringptrlist_contains(stringptrlist* list, stringptr* what) {
	size_t i;
	stringptr* s;
	if(!list || !what) return 0;
	for(i = 0; i < stringptrlist_getsize(list); i++) {
		s = stringptrlist_get(list, i);
		if(!s) return 0;
		if(stringptr_eq(what, s)) {
			return 1;
		}
	}
	return 0;
}
