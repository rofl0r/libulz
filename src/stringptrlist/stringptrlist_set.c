#include "../../include/stringptrlist.h"

int stringptrlist_set(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen) {
	stringptr* item =stringptrlist_get(l, itemnumber);
	if (!item) {
		return 0;
	}
	item->ptr = buf;
	item->size = buflen;
	return 1;
}	
