#include <stdlib.h>
#include "../../include/sblist.h"
void sblist_sort(sblist *l, int (*compar)(const void *, const void *)) {
	qsort(l->items, l->count, l->itemsize, compar);
}
