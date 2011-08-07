#include <string.h>

#include "../../include/kvlist.h"
#include "../../include/stringptr.h"

// return 1 if success
int kv_find(kvlist* list, stringptr* what, void** value) {
	size_t i;
	stringptrv* s;
	if(!list || !what || !value) return 0;
	for(i = 0; i < kv_getsize(list); i++) {
		s = kv_get(list, i);
		if(!s) return 0;
		if(stringptr_eq(what, (stringptr*) s)) {
			*value = s->value;
			return 1;
		}
	}
	return 0;
}
