#ifndef KVLIST_H
#define KVLIST_H

#include <stddef.h>
// only needed for kv_find
#include "stringptr.h"

typedef struct {
	char *ptr;
	size_t size;
	void* value;
} stringptrv;

typedef struct {
	size_t size;
	size_t capa;
} kvlist;

stringptrv* kv_get(kvlist* l, size_t itemnumber);
int kv_set(kvlist* l, size_t itemnumber, char* buf, size_t buflen, void* val);
int kv_add(kvlist** l, char* buf, size_t buflen, void* val);
kvlist* kv_new(size_t items);
kvlist* kv_resize(kvlist* list, size_t items);
int kv_find(kvlist* list, stringptr* what, void** value);

#endif
