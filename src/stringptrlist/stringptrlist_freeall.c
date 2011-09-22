#include "../../include/stringptrlist.h"

void stringptrlist_freeall(stringptrlist* l) {
	if(l) {
		stringptrlist_freestrings(l);
		stringptrlist_free(l);
	}
}
