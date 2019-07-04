#ifndef TGLIST_H
#define TGLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

/*
 * type generic list (dynamic array).
 *
 * unlike sblist, doesn't do any bounds check.
 * so you can e.g. only delete positions that are valid,
 * without causing memory corruption.
 *
 * this implementation is header-only, so it is easy
 * to embed elsewhere, however there's some overhead
 * if it's used in different TUs.
 * however the code is pretty slim, the static funcs
 * compile to about 400 byte total on x86_64.
 *
 * right now, this is in the testing stage.
 * the insert macro evaluates the pos parameter
 * twice, this will cause issues when used with
 * something like foo++.
 * functions/macros ending in _impl are not supposed
 * to be used by the user.
 *
 * the advantage of using a typed container is
 * A) the declaration of the type already documents the use
 *    (like e.g. List<int> in java)
 * B) no casts required when accessing the elements.
 * C) added type safety
 *
 * use like tglist(somename_or_id, int) *l = tglist_new();
 * the name or id can be anything that produces a valid token
 * when concatenated to "tglist_". it simply serves to give
 * the struct declaration a unique name.
 *
 * the code was designed such that the type/id is only required
 * for the declaration of the struct, not for every function call.
 * therefore unfortunately the static funcs have to resort to
 * use void* at times.
 */

#define tglist(ID, TYPE) \
	struct tglist_ ## ID { \
		size_t count; \
		size_t capa; \
		TYPE* items; \
	}

/* since the memory layout of all tglists is identical, we
   "intantiate" a prototype for accessing the members in
   void*-generic static functions. */
typedef tglist(proto, void*) tglist_proto;

#define tglist_getsize(X) ((X)->count)
#define tglist_get_count(X) ((X)->count)
#define tglist_empty(X) ((X)->count == 0)

/* --- for dynamic style --- */
// allocate and initialize a new tglist
#define tglist_new() calloc(1, sizeof(tglist_proto))

// free dynamically allocated list and its internal buffers
#define tglist_free(X) do {free((X)->items); free(X);} while(0)

/* --- for static style --- */
// initialize existing list in user-allocated storage (e.g. stack-allocated)
#define tglist_init(X) memset(X, 0, sizeof(*(X)))

// free internal buffers of the list
#define tglist_free_items(X) free((X)->items)

/* in case your list contains pointers to heap-allocated mem,
   not values, this will iterate over all list entries and
   free them */
#define tglist_free_values(X) while((X)->count > 0) \
	{free((X)->items[--((X)->count)]);}

// accessors
#define tglist_get(L, ITEM) ((L)->items[ITEM])

#define tglist_set(X, ITEM, POS) \
	((X)->items[POS] = ITEM, 1)

#define tglist_itemsize(X) sizeof( (X)->items[0] )

// returns 1 on success, 0 on OOM
#define tglist_prepare_addition(X) ( \
		tglist_grow_if_needed( X, tglist_itemsize(X) ) ? \
		!!(++(X)->count) : 0 \
	)

#define tglist_add(X, ITEM) ( \
	tglist_prepare_addition(X) ? \
	tglist_set(X, ITEM, (X)->count-1) : \
	0 )

#define tglist_ptr_from_index_impl(L, POS, ITEMSZ) \
	( (char*) ((L)->items) + (POS * ITEMSZ) )

/* void */
#define tglist_delete(X, POS) do {\
	tglist_memmove_impl(X, POS, +1, tglist_itemsize(X)); \
	--((X)->count); } while(0)

/* int : 0=err, 1=success. attention: POS is evaluated twice! */
#define tglist_insert(X, ITEM, POS) ( \
	tglist_grow_if_needed( X, tglist_itemsize(X) ) ? \
	tglist_memmove_impl(X, (POS)+1, -1, tglist_itemsize(X)) && \
		++((X)->count) && \
		tglist_set(X, ITEM, POS) \
	: 0 )

/* internal */
#define tglist_insert_memcpy(X, ITEMPTR, POS, ITEMSIZE) ( \
	tglist_grow_if_needed( X, ITEMSIZE ) ? \
	tglist_memmove_impl(X, (POS)+1, -1, ITEMSIZE) && \
		++((X)->count) && \
		memcpy(tglist_ptr_from_index_impl(X, POS, ITEMSIZE), ITEMPTR, ITEMSIZE) \
	: 0 )

/* the compare func for all sort-related stuff is qsort-style.
   note that if the list contains pointers, the compare func will get
   pointers to pointers. so to use e.g. strcmp, you need a wrapper to
   deref the const char** pointers before passing them to strcmp. */

#define tglist_sort(X, COMPAREFUNC) \
	qsort((X)->items, (X)->count, tglist_itemsize(X), COMPAREFUNC)

/* insert element into presorted list, returns listindex of new entry or -1
   due to way sorting works, we need a pointer to the item here, instead of
   the item itself.
*/
#define tglist_insert_sorted(X, ITEMPTR, COMPAREFUNC) \
	tglist_insert_sorted_impl(X, ITEMPTR, tglist_itemsize(X), COMPAREFUNC)

static int tglist_grow_if_needed(void* lst, size_t itemsize) {
	tglist_proto *l = lst;
	void* temp;
	if(l->count == l->capa) {
		size_t newsz = l->capa == 0 ? MAX(4096/itemsize, 64) : l->capa*2;
		temp = realloc(l->items, newsz * itemsize);
		if(!temp) return 0;
		l->capa = newsz;
		l->items = temp;
	}
	return 1;
}

static int tglist_memmove_impl(void *lst, size_t pos1, int pos2diff, size_t itemsz) {
	tglist_proto *l = lst;
	char* dst = tglist_ptr_from_index_impl(l, pos1, itemsz);
	const char* src = dst + (itemsz*pos2diff);
	return !!memmove(dst, src, (tglist_getsize(l) - (pos1 + pos2diff))*itemsz);
}

static size_t tglist_sorted_insert_pos_impl(void* lst, void* o, size_t itemsz, int (*compar)(const void *, const void *)) {
	tglist_proto *l = lst;
	size_t hi, lo;
	lo = tglist_getsize(l);
	if(!lo) return 0;
	lo--;
	hi = 0;
	while(1) {
		size_t c = hi + ((lo - hi) / 2);
		void *p = tglist_ptr_from_index_impl(l, c, itemsz);
		int r = compar(o, p);
		if(hi == lo) {
			if(r > 0) lo++;
			return lo;
		}
		if(r < 0) lo = c ? c-1 : 0;
		else if(r > 0) hi = c+1;
		else hi = lo = c;
		if(hi > lo) hi = lo;
	}
}

static size_t tglist_insert_sorted_impl(void *lst, void *ptr_to_item, size_t itemsz, int (*compar)(const void *, const void *)) {
	size_t idx = tglist_sorted_insert_pos_impl(lst, ptr_to_item, itemsz, compar);
	if(idx == (size_t) -1) return idx;
	if(tglist_insert_memcpy((tglist_proto*) lst, ptr_to_item, idx, itemsz)) return idx;
	return (size_t) -1;
}

#ifdef __cplusplus
}
#endif

#endif
