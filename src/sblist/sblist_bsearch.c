#include "../../include/sblist.h"
size_t sblist_bsearch(sblist *l, const void* item, int (*compar)(const void*, const void*)) {
	size_t nel = sblist_getsize(l), base = 0;
	while(nel > 0) {
		size_t x = base + nel/2;
		int c = compar(item, sblist_get(l, x));
		if (c < 0) nel /= 2;
		else if(c > 0) {
			base = x+1;
			nel -= nel/2+1;
		} else return x;
	}
	return -1L;
}

