#include "../../include/sblist.h"
#include <string.h>

int sblist_insert(sblist* l, void* item, size_t pos) {
	if(!sblist_grow_if_needed(l)) return 0;
	memmove(sblist_item_from_index(l, pos + 1), sblist_item_from_index(l, pos), (sblist_getsize(l) - pos) * l->itemsize);
	l->count++;
	return(sblist_set(l, item, pos));
}
