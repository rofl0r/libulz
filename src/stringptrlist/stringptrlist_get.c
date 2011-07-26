#include "../../include/stringptrlist.h"

stringptr* stringptrlist_get(stringptrlist* l, size_t itemnumber) {
	if(!l || itemnumber >= l->size)
		return NULL;
	return (stringptr*) (((char*)l) + sizeof(stringptrlist) + (itemnumber * sizeof(stringptr)));
}
