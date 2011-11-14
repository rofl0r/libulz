#include "../include/sblist.h"
#include <assert.h>
#include <stdio.h>

int main() {
	sblist* l = sblist_new(1, 16);
	size_t i;
	unsigned char c;
	assert(sblist_getsize(l) == 0);
	sblist_delete(l, 5);
	sblist_delete(l, 0);
	sblist_delete(l, (size_t) -1);
	
	assert(sblist_getsize(l) == 0);
	
	for(c = 0; c < 16; c++) {
		sblist_add(l, &c);
	}
	
	assert(sblist_getsize(l) == 16);
	assert(*((unsigned char*) sblist_get(l, 0)) == 0);
	assert(*((unsigned char*) sblist_get(l, 1)) == 1);
	assert(*((unsigned char*) sblist_get(l, 2)) == 2);
	assert(*((unsigned char*) sblist_get(l, 3)) == 3);
	
	sblist_delete(l, (size_t) -1);
	assert(sblist_getsize(l) == 16);
	assert(*((unsigned char*) sblist_get(l, 0)) == 0);
	assert(*((unsigned char*) sblist_get(l, 1)) == 1);
	assert(*((unsigned char*) sblist_get(l, 2)) == 2);
	assert(*((unsigned char*) sblist_get(l, 3)) == 3);
	
	
	sblist_delete(l, 5);
	sblist_delete(l, 8);
	assert(sblist_getsize(l) == 14);
	assert(*((unsigned char*) sblist_get(l, 0)) == 0);
	assert(*((unsigned char*) sblist_get(l, 1)) == 1);
	assert(*((unsigned char*) sblist_get(l, 2)) == 2);
	assert(*((unsigned char*) sblist_get(l, 3)) == 3);
	assert(*((unsigned char*) sblist_get(l, 4)) == 4);
	assert(*((unsigned char*) sblist_get(l, 5)) == 6);
	assert(*((unsigned char*) sblist_get(l, 6)) == 7);
	assert(*((unsigned char*) sblist_get(l, 7)) == 8);
	assert(*((unsigned char*) sblist_get(l, 8)) == 10);
	assert(*((unsigned char*) sblist_get(l, 9)) == 11);
	assert(*((unsigned char*) sblist_get(l, 10)) == 12);
	assert(*((unsigned char*) sblist_get(l, 11)) == 13);
	assert(*((unsigned char*) sblist_get(l, 12)) == 14);
	assert(*((unsigned char*) sblist_get(l, 13)) == 15);
	sblist_delete(l, 13);
	assert(sblist_getsize(l) == 13);
	assert(*((unsigned char*) sblist_get(l, 12)) == 14);
	assert(*((unsigned char*) sblist_get(l, 11)) == 13);
	sblist_delete(l, 0);
	assert(sblist_getsize(l) == 12);
	assert(*((unsigned char*) sblist_get(l, 0)) == 1);
	assert(*((unsigned char*) sblist_get(l, 11)) == 14);
	// try to delete non-existing record, everything should stay the same
	sblist_delete(l, 12);
	assert(sblist_getsize(l) == 12);
	assert(*((unsigned char*) sblist_get(l, 0)) == 1);
	assert(*((unsigned char*) sblist_get(l, 11)) == 14);
	
	return 0;
}