#include "../../include/stringptrlist.h"

stringptr* stringptrlist_tostring(stringptrlist* l) {
	size_t i;
	size_t req = 0;
	stringptr* result, *temp;
	for (i=0; i < stringptrlist_getsize(l); i++) {
		req += stringptrlist_get(l, i)->size + 1;
	}
	result = stringptr_new(req);
	if(!result) return NULL;
	req = 0;
	for (i=0; i < stringptrlist_getsize(l); i++) {
		temp = stringptrlist_get(l, i);
		if(temp->size) memcpy(result->ptr + req, temp->ptr, temp->size);
		req += temp->size;
		result->ptr[req] = '\n';
		req++;
	}
	return result;
}
