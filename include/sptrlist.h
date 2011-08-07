#ifndef SPTRLIST_H
#define SPTRLIST_H

#include <stddef.h>
#include "sblist.h"

/*
 * 
 * this thing here is basically a generic dynamic array
 * will realloc after every blockitems inserts
 * can store items of any size
 * 
 */
#define sptrlist sblist
#define sptrlist_new(X) sblist_new(sizeof(void*), X)
#define sptrlist_free(X) sblist_free(X)
#define sptrlist_init(X, Y) sblist_init(X, sizeof(void*), Y)
#define sptrlist_free_items(X) sblist_free_items(X)
#define sptrlist_get(X, Y) ((void**)(sblist_get(X, Y)))[0]
#define sptrlist_add(X, Y) sblist_add(X, &(Y))
#define sptrlist_set(X, Y, Z) sblist_set(X, &(Y), Z)
#define sptrlist_get_count(X) sblist_get_count(X)

#endif
