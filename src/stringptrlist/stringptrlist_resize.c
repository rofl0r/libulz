#include "../../include/stringptrlist.h"

stringptrlist* stringptrlist_resize(stringptrlist* list, size_t items) {
	if(!list) return NULL;
	stringptrlist* result;
	if((result = (stringptrlist*) realloc(list, sizeof(stringptrlist) + (sizeof(stringptr) * items))))
		result->capa = items;
	return result;
}
