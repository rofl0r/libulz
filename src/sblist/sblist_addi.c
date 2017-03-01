#include "../../include/sblist.h"

size_t sblist_addi(sblist* l, void* item) {
	if(!sblist_grow_if_needed(l)) return (size_t) -1;
	l->count++;
	sblist_set(l, item, l->count - 1);
	return l->count - 1;
}

