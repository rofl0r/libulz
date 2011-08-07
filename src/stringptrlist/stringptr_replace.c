#include "../../include/stringptrlist.h"

stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit) {
	if(!buf || !what || !whit) return NULL;
	stringptrlist* l;
	stringptr* result = NULL, *temp;
	size_t i, w;
	if((l = stringptr_splits(buf, what))) {
		result = stringptr_new(buf->size - ((stringptrlist_getsize(l) - 1) * what->size) + ((stringptrlist_getsize(l) - 1) * whit->size));
		if(!result) return NULL;
		w = 0;
		for(i = 0; i < stringptrlist_getsize(l); i++) {
			if((temp = stringptrlist_get(l, i))) {
				if(temp->size) {
					memcpy(result->ptr + w, temp->ptr, temp->size);
					w += temp->size;
				}
				if((i < stringptrlist_getsize(l) - 1) && whit->size) {
					memcpy(result->ptr + w, whit->ptr, whit->size);
					w += whit->size;
				}
			}
		}
		free(l);
	}
	return result;
}
