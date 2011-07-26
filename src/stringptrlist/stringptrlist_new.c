#include "../../include/stringptrlist.h"

stringptrlist* stringptrlist_new(size_t items) {
	stringptrlist* result = (stringptrlist*) malloc(sizeof(stringptrlist) + (sizeof(stringptr) * items));
	if(result) {
		result->size = 0;
		result->capa = items;
	}
	return result;
}
