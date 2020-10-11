#include "../../include/sblist.h"
void* sblist_pop(sblist *l) {
	size_t len = sblist_getsize(l);
	if(len > 0) {
		void *x = sblist_get(l, len-1);
		sblist_delete(l, len-1);
		return x;
	}
	return 0;
}

