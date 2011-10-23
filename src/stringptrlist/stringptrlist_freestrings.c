#include "../../include/stringptrlist.h"

void stringptrlist_freestrings(stringptrlist* l) {
	size_t i;
	stringptr* item;
	for(i = 0; i < stringptrlist_getsize(l); i++) {
		item = stringptrlist_get(l, i);
		if (item->ptr) free(item->ptr);
		item->ptr = NULL;
		item->size = 0;
	}
}
