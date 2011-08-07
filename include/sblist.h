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
#define sblist_get_count(X) ((X)->count)

#endif

//RcB: DEP "../src/sblist/*.c"
