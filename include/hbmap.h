#ifndef HBMAP_H
#define HBMAP_H

/* this is a hashmap using a fixed number of buckets,
   which in turn are of type bmap. this combines the advantages of both
   approaches.
   limitations: max no of buckets and items per bucket is 2^32-1 each.
   speed is almost identical to khash with small number of items per
   bucket. with 100.000 items it's about 15% slower.

   unlike bmap, _find(), insert(), etc return an iterator instead of indices.
   the iterator needs to be used for e.g. _getkey(), etc.
*/

#include "bmap.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h> /* ssize_t */

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

typedef uint64_t hbmap_iter;

#define hbmap_impl(NAME, KEYTYPE, VALTYPE, NUMBUCKETS) \
struct NAME { \
	unsigned (*hash_func)(const KEYTYPE); \
	union { \
		hbmap_iter it; \
	} tmp; \
	bmap_impl(, KEYTYPE, VALTYPE) buckets[NUMBUCKETS]; \
}

#define hbmap(ID, KEYTYPE, VALTYPE, NUMBUCKETS) \
	hbmap_impl(hbmap_ ## ID, KEYTYPE, VALTYPE, NUMBUCKETS)

#define hbmap_proto(NUMBUCKETS) \
	hbmap_impl(, void*, void*, NUMBUCKETS)

/* initialization */
/* bmap_compare_func is a typical compare function used for qsort, etc such as strcmp
 */
#define hbmap_init(X, COMPAREFUNC, HASHFUNC) do{\
	memset(X, 0, sizeof(*(X))); \
	size_t i; for(i=0; i<ARRAY_SIZE((X)->buckets); ++i) \
		(X)->buckets[i].compare = COMPAREFUNC; \
	(X)->hash_func = HASHFUNC; \
	} while(0)

static inline void* hbmap_new(bmap_compare_func fn, void* hash_func, size_t numbuckets) {
	hbmap_proto(numbuckets) *nyu = malloc(sizeof(hbmap_proto(numbuckets)));
	if(nyu) hbmap_init(nyu, fn, hash_func);
	return nyu;
}

/* destruction */
/* freeflags:
  0: free only internal mem
  1: 0+free all keys,
  2: 0+free all values,
  3: 0+free both
*/
#define hbmap_fini(X, FREEFLAGS) do { \
	size_t i; for(i=0; i<ARRAY_SIZE((X)->buckets); ++i) \
		{ bmap_fini(&(X)->buckets[i], FREEFLAGS); } \
} while(0)

#define hbmap_getbucketcount(X) ARRAY_SIZE((X)->buckets)

/* internal stuff needed for iterator impl */

#define hbmap_iter_bucket(I) ( (I) >> 32)
#define hbmap_iter_index(I)  ( (I) & 0xffffffff )
#define hbmap_iter_makebucket(I) ( (I) << 32)

#define hbmap_iter_bucket_valid(X, ITER) ( \
	hbmap_iter_bucket(ITER) < hbmap_getbucketcount(X) )
#define hbmap_iter_index_valid(X, ITER) ( \
	hbmap_iter_index(ITER) < bmap_getsize(&(X)->buckets[hbmap_iter_bucket(ITER)]) )
#define hbmap_iter_valid(X, ITER) \
	(hbmap_iter_bucket_valid(X, ITER) && hbmap_iter_index_valid(X, ITER))

#define hbmap_next_step(X, ITER) ( \
	hbmap_iter_index_valid(X, (ITER)+1) ? (ITER)+1 : \
	hbmap_iter_makebucket(hbmap_iter_bucket(ITER)+1) \
	)

static hbmap_iter hbmap_next_valid_impl(void *map, hbmap_iter iter, size_t nbucks) {
	hbmap_proto(nbucks) *h = map;
	do iter = hbmap_next_step(h, iter);
	while(hbmap_iter_bucket_valid(h, iter) && !hbmap_iter_index_valid(h, iter));
	return iter;
}

/* public API continues */

#define hbmap_foreach(X, ITER_VAR) \
	for(ITER_VAR = 0; hbmap_iter_bucket_valid(X, ITER_VAR); \
		ITER_VAR = hbmap_next_valid_impl(X, ITER_VAR, hbmap_getbucketcount(X)))

#define hbmap_getkey(X, ITER) \
	bmap_getkey(&(X)->buckets[hbmap_iter_bucket(ITER)], hbmap_iter_index(ITER))

#define hbmap_getval(X, ITER) \
	bmap_getval(&(X)->buckets[hbmap_iter_bucket(ITER)], hbmap_iter_index(ITER))

#define hbmap_setvalue(X, VAL, ITER) \
	bmap_setvalue(&(X)->buckets[hbmap_iter_bucket(ITER)], VAL, hbmap_iter_index(ITER))

#define hbmap_getkeysize(X) (bmap_getkeysize(&(X)->buckets[0]))
#define hbmap_getvalsize(X) (bmap_getvalsize(&(X)->buckets[0]))

#define hbmap_buckindex(X, KEY) \
	( (X)->hash_func(KEY) % hbmap_getbucketcount(X) )

#define hbmap_find(X, KEY) ( \
	( (X)->tmp.it = hbmap_iter_makebucket(hbmap_buckindex(X, KEY) ) ), \
	((X)->tmp.it |= (int64_t) bmap_find(&(X)->buckets[ hbmap_iter_bucket((X)->tmp.it) ], KEY)), \
	(X)->tmp.it)

#define hbmap_contains(X, KEY) (hbmap_find(X, KEY) != (hbmap_iter)-1)

/* unlike hbmap_getkey/val with index, this returns a pointer-to-item, or NULL */
#define hbmap_get(X, KEY) ( \
	( hbmap_find(X, KEY) == (hbmap_iter) -1 ) ? 0 : &hbmap_getval(X, (X)->tmp.it) \
	)

/* same as hbmap_insert, but inserts blindly without checking for existing items.
   this is faster and can be used when it's impossible that duplicate
   items are added */
#define hbmap_insert_nocheck(X, KEY, VAL) ( \
	( (X)->tmp.it = hbmap_iter_makebucket(hbmap_buckindex(X, KEY) ) ), \
	((X)->tmp.it |= (int64_t) bmap_insert_nocheck(&(X)->buckets[hbmap_iter_bucket((X)->tmp.it)], KEY, VAL)), \
	(X)->tmp.it)

/* insert item into mapping, overwriting existing items with the same key */
/* return index of new item, or -1. overwrites existing items. */
#define hbmap_insert(X, KEY, VAL) ( \
		( hbmap_find(X, KEY) == (hbmap_iter) -1 ) ? hbmap_insert_nocheck(X, KEY, VAL) : \
		( hbmap_setvalue(X, VAL, (X)->tmp.it), (X)->tmp.it ) \
	)

#define hbmap_delete(X, ITER) ( \
	bmap_delete(&(X)->buckets[hbmap_iter_bucket(ITER)], hbmap_iter_index(ITER)), 1)

#endif