#include "../../include/stringptrlist.h"

// return 1 if success
int stringptrlist_contains(stringptrlist* list, stringptr* what) {
	return stringptrlist_find(list, what) != -1;
}
