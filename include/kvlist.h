#ifndef KVLIST_H
#define KVLIST_H

#include <stddef.h>
// only needed for kv_find
#include "stringptr.h"
#include "sblist.h"

typedef struct {
	char *ptr;
	size_t size;
	void* value;
} stringptrv;

#define SPVMAKE(X, Y, Z) (&(stringptrv) {(X), (Y), (Z)})

#define kvlist sblist
//stringptrv* kv_get(kvlist* l, size_t itemnumber);
#define kv_get(X, Y) ((stringptrv*) (sblist_get((X), (Y))))
//int kv_set(kvlist* l, size_t itemnumber, char* buf, size_t buflen, void* val);
#define kv_set(V, W, X, Y, Z) sblist_set((V), SPVMAKE((X), (Y), (Z)), (W))
//int kv_add(kvlist** l, char* buf, size_t buflen, void* val);
#define kv_add(W, X, Y, Z) sblist_add((W), SPVMAKE((X), (Y), (Z)))
//kvlist* kv_new(size_t items);
#define kv_new(X) sblist_new(sizeof(stringptrv), (X))
//kvlist* kv_resize(kvlist* list, size_t items);
#define kv_getsize(X) ((X)->count)

int kv_find(kvlist* list, stringptr* what, void** value);

#endif

//RcB: DEP "../src/kvlist/*.c"
