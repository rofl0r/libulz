#include "../include/tglist.h"
#include <assert.h>
#include <stdio.h>

int main() {
	tglist(_, unsigned char) *l = tglist_new();
	size_t i;
	unsigned char c;
	assert(tglist_getsize(l) == 0);

	for(c = 0; c < 16; c++) {
		tglist_add(l, c);
	}

	assert(tglist_getsize(l) == 16);
	assert(tglist_get(l, 0) == 0);
	assert(tglist_get(l, 1) == 1);
	assert(tglist_get(l, 2) == 2);
	assert(tglist_get(l, 3) == 3);


	tglist_delete(l, 5);
	tglist_delete(l, 8);
	assert(tglist_getsize(l) == 14);
	assert(tglist_get(l, 0) == 0);
	assert(tglist_get(l, 1) == 1);
	assert(tglist_get(l, 2) == 2);
	assert(tglist_get(l, 3) == 3);
	assert(tglist_get(l, 4) == 4);
	assert(tglist_get(l, 5) == 6);
	assert(tglist_get(l, 6) == 7);
	assert(tglist_get(l, 7) == 8);
	assert(tglist_get(l, 8) == 10);
	assert(tglist_get(l, 9) == 11);
	assert(tglist_get(l, 10) == 12);
	assert(tglist_get(l, 11) == 13);
	assert(tglist_get(l, 12) == 14);
	assert(tglist_get(l, 13) == 15);

	tglist_delete(l, 13);
	assert(tglist_getsize(l) == 13);
	assert(tglist_get(l, 12) == 14);
	assert(tglist_get(l, 11) == 13);

	tglist_delete(l, 0);
	assert(tglist_getsize(l) == 12);
	assert(tglist_get(l, 0) == 1);
	assert(tglist_get(l, 11) == 14);

	return 0;
}
