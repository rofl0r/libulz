#ifndef TLIST_H
#define TLIST_H

#include <stddef.h>

/*
   tlist, aka treap list. (C) 2024 rofl0r.

   the core's algorithm is based on the "implicit treap" described on
   e-maxx.ru.

   tlist behaves like a dynamic array, but unlike a real array supports
   insertion and deletion with O(log N) performance characteristics.

   indexing is also O(log N), so if you don't need fast insertion and
   deletion, only appending, picking a traditional dynamic array will be
   faster for your usecase.

   on a fast machine with plenty of cache, a traditional dynamic array
   will also be faster if you need to insert or remove some items, and
   the array is reasonably small (up to about 2k entries).
   though even there the tlist is quite competitive.
   for anything else, the tlist outperforms the traditional array by far.

   its most appealing characteristic, aside from the above, is that it
   can be implemented in less than 100 lines.
   it comes at a cost though.
   the memory consumption per node is 24 bytes on 64bit machines, and 16
   on 32 bit. plus the size of the item being stored in it.

   the list is initialized with the fixed size of a single item that
   needs to be stored - it could be a single integer, a struct,
   or a pointer.

   on insertion, you pass a pointer to a single item, the object being
   pointed to is then copied into the node.

   for non-fixed size data items such as strings, you'd use the size
   of a pointer for tlist_new(). then you need to allocate the strings
   yourself and insert a pointer to the string - i.e. a char**.
   the list can free the pointed to content automatically if you use
   the _deep suffixed functions.

   the list can hold a maximum of UINT_MAX items.

   note that unlike in my dynamic array implementation "sblist", functions
   taking an index receive it as second, not third argument.
   it seems more natural to first pass the list, then the index, then the
   value, as the index refers to the list.
   apart from that, the api is almost identical, which allows for a quick
   swap-out.
*/

struct tlist;
typedef struct tlist tlist;

/* allocates a new list prepared to store nodes of itemsize size.
   may return NULL on resource exhaustion. */
struct tlist *tlist_new(unsigned itemsize);

/* return the number of items/nodes in the list. */
size_t tlist_getsize(struct tlist* t);

/* get the pointer to the data of idx'th item in the list.
   may return NULL if the idx is equal or greater than list size,
   you have to cast the return value to a pointer to the type
   that you inserted. */
void *tlist_get(struct tlist* t, size_t idx);

/* insert value at position idx */
/* returns 1 on success, 0 otherwise (i.e. not enough ram) */
int tlist_insert(struct tlist* t, size_t idx, void* val);

/* append value to the end of the list */
int tlist_append(struct tlist* t, void *val);

/* delete item as position idx */
/* returns 1 on success, 0 otherwise (invalid index) */
int tlist_delete(struct tlist *t, size_t idx);
/* same as tlist_delete, but frees the stored pointer too - only use
   if you initialized the list with sizeof(pointer) */
int tlist_delete_deep(struct tlist *t, size_t idx);

/* remove and free all items in list, but not the list itself. */
void tlist_free_items(struct tlist *t);
void tlist_free_items_deep(struct tlist *t);

/* free the list and all items in it - returns NULL so you can do
   mylist = tlist_free(mylist) instead of requiring 2 statements to
   have your list freed and nulled. */
void* tlist_free(struct tlist *t);
void* tlist_free_deep(struct tlist *t);

/* this is just a debug function that prints the node balance of
   the tree. it's not built-in by default because it prints stuff
   to stdout. */
float tlist_getbalance(struct tlist *t);

#pragma RcB2 DEP "../src/tlist/*.c"

#endif

