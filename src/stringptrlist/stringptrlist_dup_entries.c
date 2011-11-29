#include "../../include/stringptrlist.h"

ssize_t stringptrlist_dup_entries(stringptrlist* l) {
	size_t i;
	stringptr* item;
	char* duped;
	for(i = 0; i < stringptrlist_getsize(l); i++) {
		item = stringptrlist_get(l, i);
		duped = stringptr_strdup(item);
		if(!duped) return i;
		item->ptr = duped;
	}
	return -1;
}
