#ifndef SBLIST_H
#define SBLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
/*
 * simple buffer list.
 *
 * this thing here is basically a generic dynamic array (like a C++ vector).
 * all data is stored in a single contiguous block of memory.
 *
 * will realloc after every blockitems inserts
 * can store items of any size.
 *
 * so think of it as a by-value list, as opposed to a typical by-ref list.
 * you typically use it by having some struct on the stack, and pass a pointer
 * to sblist_add, which will copy the contents into its internal memory.
 *
 */

typedef struct {
	size_t itemsize;
	size_t blockitems;
	size_t count;
	size_t capa;
	char* items;
} sblist;

#define sblist_getsize(X) ((X)->count)
#define sblist_get_count(X) ((X)->count)
#define sblist_empty(X) ((X)->count == 0)

/* --- for dynamic style --- */

/* allocate and initialize a new sblist. blockitems denotes how many items
   to allocate at a time. using bigger values will reduce realloc overhead,
   at the cost of higher memory usage. if 0 is passed, a new page of 4KB will
   be requested whenever the list needs to grow. */
sblist* sblist_new(size_t itemsize, size_t blockitems);

/* allocate and init a sblist from an existing contiguous chunk of memory.
   if that list is to be used for both read and write operations, it needs
   to be allocated by a libc function compatible with further usage of realloc.
   blockitems will be set to 0 - see above for what that means.
   if a different blockitems value is needed (only makes sense in r/w mode),
   it may be set using sblist_set_blockitems().
 */
sblist *sblist_new_from_data(size_t itemsize, void *data, size_t datalen);
#define sblist_set_blockitems(X, N) do { (X)->blockitems = N; } while(0)

/* free dynamically allocated list and its internal buffers */
void sblist_free(sblist* l);

/* --- for static style --- */
/* initialize existing sblist in user-allocated storage (e.g. stack-allocated)*/
void sblist_init(sblist* l, size_t itemsize, size_t blockitems);
/* free internal buffers of the list */
void sblist_free_items(sblist* l);

/* in case your list contains pointers, not values, this will
   iterate over all list entries and free them */
void sblist_free_values(sblist *l);

/* accessors */
void* sblist_get(sblist* l, size_t item);

/* returns 1 on success, 0 on OOM */
int sblist_add(sblist* l, void* item);
int sblist_set(sblist* l, void* item, size_t pos);
void sblist_delete(sblist* l, size_t item);
char* sblist_item_from_index(sblist* l, size_t idx);
int sblist_grow_if_needed(sblist* l);
int sblist_insert(sblist* l, void* item, size_t pos);

/* you can use sblist as a stack by using sblist_add()/sblist_pop() */
/* return last item in list and remove it. returns NULL if no items in list. */
void* sblist_pop(sblist *l);

/* same as sblist_add, but returns list index of new item, or -1 */
size_t sblist_addi(sblist* l, void* item);

/* uses qsort() to sort list entries - it's probably a better idea to insert
   them sorted to begin with using sblist_insert_sorted() - which doesn't pull
   in libc's qsort(), and is faster. */
void sblist_sort(sblist *l, int (*compar)(const void *, const void *));

/* for all functions below using a compare function the following rule applies:
   the searched-for item will be passed to the compare function as first
   argument, the pivot item as second.
   in case of sblist_sort, the order is unspecified, as it depends on the
   implementation details of libc qsort().
*/

/* insert element into presorted list, returns listindex of new entry or -1*/
size_t sblist_insert_sorted(sblist* l, void* o, int (*compar)(const void *, const void *));
/* same as above, but provides a context pointer passed to qsort_r-style 3-arg
   compare function. */
size_t sblist_insert_sorted_r(sblist* l, void* o, int (*compar)(const void *, const void *, void*), void* ctx);

/* binary-searches for item in a sorted list using compar func.
   returns -1L if not found, otherwise index. */
size_t sblist_bsearch(sblist *l, const void* item, int (*compar)(const void*, const void*));
/* same as above, but provides a context pointer passed to qsort_r-style 3-arg
   compare function. */
size_t sblist_bsearch_r(sblist *l, const void* item, int (*compar)(const void*, const void*, void*), void* ctx);

#ifndef __COUNTER__
#define __COUNTER__ __LINE__
#endif

#define __sblist_concat_impl( x, y ) x##y
#define __sblist_macro_concat( x, y ) __sblist_concat_impl( x, y )
#define __sblist_iterator_name __sblist_macro_concat(sblist_iterator, __COUNTER__)

/* use with custom iterator variable */
#define sblist_iter_counter(LIST, ITER, PTR) \
	for(size_t ITER = 0; (PTR = sblist_get(LIST, ITER)), ITER < sblist_getsize(LIST); ITER++)

/* use with custom iterator variable, which is predeclared */
#define sblist_iter_counter2(LIST, ITER, PTR) \
	for(ITER = 0; (PTR = sblist_get(LIST, ITER)), ITER < sblist_getsize(LIST); ITER++)

/* use with custom iterator variable, which is predeclared and signed */
/* useful for a loop which can delete items from the list, and then decrease the iterator var. */
#define sblist_iter_counter2s(LIST, ITER, PTR) \
	for(ITER = 0; (PTR = sblist_get(LIST, ITER)), ITER < (ssize_t) sblist_getsize(LIST); ITER++)


/* uses "magic" iterator variable */
#define sblist_iter(LIST, PTR) sblist_iter_counter(LIST, __sblist_iterator_name, PTR)

#ifdef __cplusplus
}
#endif

#pragma RcB2 DEP "../src/sblist/*.c"

#endif
