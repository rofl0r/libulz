#include "../../include/stringptrlist.h"

int stringptrlist_add_strdup(stringptrlist* l, stringptr* s) {
	char* nu = stringptr_strdup(s);
	if(!nu) return 0;
	return stringptrlist_add(l, nu, stringptr_getsize(s));
}
