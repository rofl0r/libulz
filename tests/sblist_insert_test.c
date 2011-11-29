#include "../include/sblist.h"
#include <assert.h>

int main() {
	int i = 0;
	sblist* l = sblist_new(sizeof(int), 4);
	assert(sblist_getsize(l) == 0);
	sblist_add(l, &i);
	assert(sblist_getsize(l) == 1);
	i++;
	sblist_add(l, &i);
	assert(sblist_getsize(l) == 2);
	i++;
	sblist_add(l, &i);
	assert(sblist_getsize(l) == 3);
	i++;
	sblist_insert(l, &i, 2);
	assert(sblist_getsize(l) == 4);
	i++;
	assert(*((int*)sblist_get(l, 0)) == 0);
	assert(*((int*)sblist_get(l, 1)) == 1);
	assert(*((int*)sblist_get(l, 2)) == 3);
	assert(*((int*)sblist_get(l, 3)) == 2);
	
	assert(sblist_getsize(l) == 4);
	
	//sblist_free(l);
	l = sblist_new(sizeof(int), 1);
	assert(sblist_getsize(l) == 0);
	
	i = 0;
	
	sblist_insert(l, &i, i);
	assert(sblist_getsize(l) == 1);
	i++;
	sblist_insert(l, &i, i);
	assert(sblist_getsize(l) == 2);
	i++;
	sblist_insert(l, &i, i);
	assert(sblist_getsize(l) == 3);
	i++;
	sblist_insert(l, &i, i);
	assert(sblist_getsize(l) == 4);
	i++;
	
	assert(*((int*)sblist_get(l, 0)) == 0);
	assert(*((int*)sblist_get(l, 1)) == 1);
	assert(*((int*)sblist_get(l, 2)) == 2);
	assert(*((int*)sblist_get(l, 3)) == 3);
	
	l = sblist_new(sizeof(int), 1);
	assert(sblist_getsize(l) == 0);
	
	i = 0;
	
	sblist_insert(l, &i, 0);
	assert(sblist_getsize(l) == 1);
	i++;
	sblist_insert(l, &i, 0);
	assert(sblist_getsize(l) == 2);
	i++;
	sblist_insert(l, &i, 0);
	assert(sblist_getsize(l) == 3);
	i++;
	sblist_insert(l, &i, 0);
	assert(sblist_getsize(l) == 4);
	i++;
	
	assert(*((int*)sblist_get(l, 0)) == 3);
	assert(*((int*)sblist_get(l, 1)) == 2);
	assert(*((int*)sblist_get(l, 2)) == 1);
	assert(*((int*)sblist_get(l, 3)) == 0);
	
	return 0;
}
