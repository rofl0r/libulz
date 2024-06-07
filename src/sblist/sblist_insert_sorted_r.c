#include "../../include/sblist.h"
#include <stddef.h>

size_t sblist_insert_sorted_r(sblist* l, void* o, int (*compar)(const void*, const void*, void*), void *ctx) {
	size_t hi, lo;
	lo = sblist_getsize(l);
	if(!lo) goto ins;
	lo--;
	hi = 0;
	while(1) {
		size_t c = hi + ((lo - hi) / 2);
		void *p = sblist_get(l, c);
		int r = compar(o, p, ctx);
		if(hi == lo) {
			if(r > 0) lo++;
	ins:
			if(sblist_insert(l, o, lo)) return lo;
			else return (size_t)-1;
		}
		if(r < 0) lo = c ? c-1 : 0;
		else if(r > 0) hi = c+1;
		else hi = lo = c;
		if(hi > lo) hi = lo;
	}
}
