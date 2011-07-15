#include <string.h>

#include "kvlist.h"
#include "stringptr.h"
//RcB: DEP "stringptr.c"

stringptrv* kv_get(kvlist* l, size_t itemnumber) {
	if(!l || itemnumber >= l->size)
		return NULL;
	return (stringptrv*) (((char*)l) + sizeof(kvlist) + (itemnumber * sizeof(stringptrv)));
}

//return 1 if success
int kv_set(kvlist* l, size_t itemnumber, char* buf, size_t buflen, void* val) {
	stringptrv* item;	
	if(!l || !(item = kv_get(l, itemnumber))) return 0;
	item->ptr = buf;
	item->size = buflen;
	item->value = val;
	return 1;
}

int kv_add(kvlist** l, char* buf, size_t buflen, void* val) {
	kvlist* nl = NULL;
	size_t nl_flag = 0;
	if(!l || !*l) return 0;
	if((*l)->size + 1 >= (*l)->capa && (nl_flag = 1) && !(nl = kv_resize((*l), (*l)->capa * 2))) return 0;
	if(nl_flag) {
		if(nl) *l = nl;
		else return 0;
	}	
	(*l)->size++;
	return kv_set((*l), (*l)->size - 1, buf, buflen, val);
}

kvlist* kv_new(size_t items) {
	kvlist* result = (kvlist*) malloc(sizeof(kvlist) + (sizeof(stringptrv) * items));
	if(result) {
		result->size = 0;
		result->capa = items;
	}
	return result;
}

kvlist* kv_resize(kvlist* list, size_t items) {
	if(!list) return NULL;
	kvlist* result;
	if((result = (kvlist*) realloc(list, sizeof(kvlist) + (sizeof(stringptrv) * items))))
		result->capa = items;
	return result;
}

// return 1 if success
int kv_find(kvlist* list, stringptr* what, void** value) {
	size_t i;
	stringptrv* s;
	if(!list || !what || !value) return 0;
	for(i = 0; i < list->size; i++) {
		s = kv_get(list, i);
		if(!s) return 0;
		if(streq(what, (stringptr*) s)) {
			*value = s->value;
			return 1;
		}	
	}
	return 0;
}