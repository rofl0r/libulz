#include "../../include/sblist.h"
#include <string.h>

void sblist_delete(sblist* l, size_t item) {
	size_t i, m;
	if (l->count && item < l->count) {
		m = l->count - 1;
		for(i = item; i < m; i++) {
			memcpy(sblist_get(l, i), sblist_get(l, i + 1), l->itemsize);
		}
		l->count--;
	}
}
