#ifndef SBLIST_H
#define SBLIST_H

#include <stddef.h>

/*
 * 
 * this thing here is basically a generic dynamic array
 * will realloc after every blockitems inserts
 * can store items of any size
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

// for dynamic style
sblist* sblist_new(size_t itemsize, size_t blockitems);
void sblist_free(sblist* l);

//for static style
void sblist_init(sblist* l, size_t itemsize, size_t blockitems);
void sblist_free_items(sblist* l);

// accessors
void* sblist_get(sblist* l, size_t item);
// returns 1 on success, 0 on OOM
int sblist_add(sblist* l, void* item);
int sblist_set(sblist* l, void* item, size_t pos);
void sblist_delete(sblist* l, size_t item);
char* sblist_item_from_index(sblist* l, size_t idx);
int sblist_grow_if_needed(sblist* l);
int sblist_insert(sblist* l, void* item, size_t pos);


#define __sblist_concat_impl( x, y ) x##y
#define __sblist_macro_concat( x, y ) __sblist_concat_impl( x, y )
#define __sblist_iterator_name __sblist_macro_concat(sblist_iterator, __COUNTER__)

// use with custom iterator variable
#define sblist_iter_counter(LIST, ITER, PTR) size_t ITER; \
	for(ITER = 0; (PTR = sblist_get(LIST, ITER)), ITER < sblist_getsize(LIST); ITER++)

// uses "magic" iterator variable
#define sblist_iter(LIST, PTR) sblist_iter_counter(LIST, __sblist_iterator_name, PTR)

#endif

//RcB: DEP "../src/sblist/*.c"
