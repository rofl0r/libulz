#include "../../include/stringptr.h"

int stringptr_eq(stringptr* a, stringptr* b) {
	if(!a && !b) return 1;
	if((a && !b) || (!a && b)) return 0;
	return (a->size == b->size && !memcmp(a->ptr, b->ptr, a->size));
}
