#ifndef BMAP_H
#define BMAP_H

/* this is a map behaving like a hashmap, but it uses binary search
   on a sorted list behind the curtains. this allows to find the
   required entry very quickly, while avoiding a lot of the complexity
   of a hashmap.

   for e.g. 10000 array elements, a binary search will only require 12
   comparisons, whereas a hashmap needs to compute a hash, then find
   a corresponding bucket and iterate over the bucket items, of which
   there could be multiple as well. so it shouldn't be much faster,
   while having a lot more overhead in code and RAM.

   since we use our per-value sblist behind the scenes, which looks
   basically like a flat array of the stored items, there's almost
   zero memory overhead with our method here.

   the slowest part is insertion of the item into the list, which uses
   memmove on a single block of memory. this is no problem when the
   total number of entries is relatively small.

   when comparing to khash, which is known as one of the fastest hashmap
   implementations due to usage of macros to generate inlined code,
   we reach about 80-75% of its speed with around 1000 items, but only
   40% with 3000, 33% with 10000 etc. the more items, the slower it
   becomes in comparison.

   so for the vast majority of tasks, this implementation provides
   speed comparable to the fastest hashmap implementation, while adding
   only a few hundred byte to binary size. a size-optimized benchmark
   program with bmap is 5.5KB, an equivalent one with kash 9.6KB.

   an additional advantage is that the map can be iterated like a
   regular array, which is already sorted by key.

*/

#include "sblist.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h> /* ssize_t */

typedef int (*bmap_compare_func)(const void *, const void *);

typedef struct bmap {
	sblist keys;
	sblist values;
	bmap_compare_func compare;
} bmap;

/* initialization */
/* bmap_compare_func is a typical compare function used for qsort, etc such as strcmp
   flags ... */
void bmap_init(bmap* b, size_t keysize, size_t valsize, bmap_compare_func);
static inline bmap* bmap_new(size_t keysize, size_t valsize, bmap_compare_func fn, int flags) {
	bmap *nyu = malloc(sizeof(bmap));
	if(nyu) bmap_init(nyu, keysize, valsize, fn);
	return nyu;
}
/* destruction */
/* freeflags:
  0: free only internal mem
  1: 0+free all keys,
  2: 0+free all values,
  3: 0+free both
*/
void bmap_fini(bmap *b, int freeflags);

/* return index of new item, or -1. overwrites existing items. */
ssize_t bmap_insert(bmap *b, const void *key, const void *value);
/* same as above, but inserts blindly without checking for existing items.
   this is 2x faster and can be used when it's impossible that duplicate
   items are added */
ssize_t bmap_insert_nocheck(bmap *b, const void *key, const void *value);

/* set value when key index is known. returns int 0 on failure, 1 on succ.*/
#define bmap_setvalue(B, VAL, POS) sblist_set(&(B)->values, VAL, POS)

#define bmap_getsize(B) sblist_getsize(&(B)->keys)
#define bmap_getkey(B, X) sblist_get(&(B)->keys, X)
#define bmap_getval(B, X) sblist_get(&(B)->values, X)
#define bmap_getkeysize(B) ((B)->keys.itemsize)
#define bmap_getvalsize(B) ((B)->keys.itemsize)

ssize_t bmap_find(bmap* b, const void* key);

#define bmap_contains(B, KEY) (bmap_find(B, KEY) != (ssize_t)-1)

static inline void* bmap_get(bmap* b, void *key) {
	ssize_t idx = bmap_find(b, key);
	if(idx == (ssize_t) -1) return 0;
	return bmap_getval(b, idx);
}

#pragma RcB2 DEP "../src/bmap/bmap.c"

#endif
