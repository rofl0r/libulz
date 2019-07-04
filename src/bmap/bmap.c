#include "../../include/bmap.h"
#include "../../include/macros.h"
#include <stdlib.h>
#include <stdint.h>

void bmap_init(bmap* b, size_t keysize, size_t valsize, bmap_compare_func fn) {
	size_t items = MAX(4096/MAX(keysize, valsize), 64);
	sblist_init(&b->keys, keysize, items);
	sblist_init(&b->values, valsize, items);
	b->compare = fn;
}

ssize_t bmap_find(bmap* b, const void* key) {
	void *r = bsearch(key, b->keys.items, bmap_getsize(b), bmap_getkeysize(b), b->compare);
	if(!r) return -1;
	return ((uintptr_t) r - (uintptr_t) b->keys.items)/bmap_getkeysize(b);
}

ssize_t bmap_insert_nocheck(bmap *b, const void *key, const void *value) {
	ssize_t idx = sblist_insert_sorted(&b->keys, key, b->compare);
	if(idx >= 0 && !sblist_insert(&b->values, value, idx)) {
		/* inserting into keys succeeded, but values not.
		   in order to have a consistent state, we need to
		   delete the key again */
		sblist_delete(&b->keys, idx);
		idx = -1;
	}
	return idx;
}

ssize_t bmap_insert(bmap *b, const void *key, const void *value) {
	ssize_t idx = bmap_find(b, key);
	if(idx >= 0) sblist_set(&b->values, value, idx);
	else idx = bmap_insert_nocheck(b, key, value);
	return idx;
}

void bmap_fini(bmap *b, int freeflags) {
	size_t i;
	if(freeflags & 1) sblist_free_values(&b->keys);
	if(freeflags & 2) sblist_free_values(&b->values);
	sblist_free_items(&b->keys);
	sblist_free_items(&b->values);
}
