#include "../include/tglist.h"
#include <assert.h>

static int intcmp(const void *vp1, const void* vp2) {
	const int* a = vp1;
	const int* b = vp2;
	return *a - *b;
}

static int mystrstrcmp(const void *vp1, const void* vp2) {
	const char * const *a = vp1;
	const char * const *b = vp2;
	return strcmp(*a, *b);
}

int strtest() {
	tglist(char*) l_buf, *l = &l_buf;
	tglist_init(l);
	const char* org[] = {
		"Lorem ipsum dolor sit amet, ",
		"consectetur adipiscing elit, ",
		"sed do eiusmod tempor incididunt ",
		"ut labore et dolore magna aliqua. ",
		"Ut enim ad minim veniam, ",
		"quis nostrud exercitation ullamco laboris ",
		"nisi ut aliquip ex ea commodo consequat. ",
		"Duis aute irure dolor in reprehenderit ",
		"in voluptate velit esse cillum dolore ",
		"eu fugiat nulla pariatur. ",
		"Excepteur sint occaecat cupidatat non proident, ",
		"sunt in culpa qui officia deserunt mollit ",
		"anim id est laborum.",
		0
	};
	size_t i;
	for(i = 0; org[i]; i++) {
		tglist_add(l, strdup(org[i]));
	}
	for(i = 0; org[i]; i++)
		assert(!strcmp(org[i], tglist_get(l, i)));

	tglist_sort(l, mystrstrcmp);
	for(i = 0; org[i]; i++) {
		const char* tmp = strdup(org[i]);
		tglist_insert_sorted(l, tmp, mystrstrcmp);
	}
	assert(!strncmp("Duis", tglist_get(l, 0), 4));
	assert(!strncmp("Duis", tglist_get(l, 1), 4));

	tglist_delete(l, 1);
	tglist_delete(l, 0);

	assert( strncmp("Duis", tglist_get(l, 0), 4));


	tglist_free_values(l);
	tglist_free_items(l);
	return 0;
}

int main() {
	int i = 0;
	tglist(int) *l = tglist_new();

	assert(tglist_getsize(l) == 0);
	tglist_add(l, i);
	assert(tglist_getsize(l) == 1);
	i++;
	tglist_add(l, i);
	assert(tglist_getsize(l) == 2);
	i++;
	tglist_add(l, i);
	assert(tglist_getsize(l) == 3);
	i++;

	/* i is 3 at this point */
	tglist_insert(l, i, 2);
	assert(tglist_getsize(l) == 4);
	i++;
	assert(tglist_get(l, 0) == 0);
	assert(tglist_get(l, 1) == 1);
	assert(tglist_get(l, 2) == 3);
	assert(tglist_get(l, 3) == 2);

	assert(tglist_getsize(l) == 4);

	tglist_free(l);
	l = tglist_new();
	assert(tglist_getsize(l) == 0);

	i = 0;
	for(i=0; i<16384; i++) {
		tglist_insert(l, i, i);
		assert(tglist_getsize(l) == i+1);
	}

	assert(tglist_get(l, 0) == 0);
	assert(tglist_get(l, 1) == 1);
	assert(tglist_get(l, 2) == 2);
	assert(tglist_get(l, 3) == 3);

	assert(tglist_get(l, 16000) == 16000);

	i = 3;
	assert(tglist_insert_sorted(l, i, intcmp));
	assert(tglist_get(l, 3) == 3);
	assert(tglist_get(l, 4) == 3);

	assert(tglist_get(l, 16000) == 15999);




	l = tglist_new();
	assert(tglist_getsize(l) == 0);

	i = 0;

	tglist_insert(l, i, 0);
	assert(tglist_getsize(l) == 1);
	i++;
	tglist_insert(l, i, 0);
	assert(tglist_getsize(l) == 2);
	i++;
	tglist_insert(l, i, 0);
	assert(tglist_getsize(l) == 3);
	i++;
	tglist_insert(l, i, 0);
	assert(tglist_getsize(l) == 4);
	i++;

	assert(tglist_get(l, 0) == 3);
	assert(tglist_get(l, 1) == 2);
	assert(tglist_get(l, 2) == 1);
	assert(tglist_get(l, 3) == 0);

	return strtest();
}
