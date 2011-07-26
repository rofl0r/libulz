#include "../../include/stringptrlist.h"

int stringptrlist_add(stringptrlist** l, char* s, size_t len) {
	stringptrlist* b;
	if((*l)->size + 1 >= (*l)->capa) {
		if(!(b = stringptrlist_resize(*l, (*l)->capa * 2))) return 0;
		*l = b;
	}
	(*l)->size++;
	if(!(stringptrlist_set(*l, (*l)->size - 1, s, len))) {
		(*l)->size--;
		return 0;
	}
	return 1;
}
