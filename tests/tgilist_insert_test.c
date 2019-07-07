#include "../include/tgilist.h"
#include "../include/macros.h"
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
	tgilist(char*, unsigned short) l_buf, *l = &l_buf;
	tgilist_init(l);
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
		tgilist_add(l, strdup(org[i]));
	}
	for(i = 0; org[i]; i++)
		assert(!strcmp(org[i], tgilist_get(l, i)));

	tgilist_insert(l, strdup(org[7]), 0);
	tgilist_insert(l, strdup(org[7]), 0);

	assert(tgilist_getsize(l) == ARRAY_SIZE(org)-1+2);

	assert(!strncmp("Duis", tgilist_get(l, 0), 4));
	assert(!strncmp("Duis", tgilist_get(l, 1), 4));

	tgilist_delete(l, 1);
	tgilist_delete(l, 0);

	assert( strncmp("Duis", tgilist_get(l, 0), 4));
	assert( tgilist_getsize(l) == ARRAY_SIZE(org)-1);

	for(i = 0; org[i]; i++)
		assert(!strcmp(org[i], tgilist_get(l, i)));

	tgilist_delete(l, 1);
	assert(!strcmp(org[0], tgilist_get(l, 0)));
	assert(!strcmp(org[2], tgilist_get(l, 1)));

	assert( tgilist_getsize(l) == ARRAY_SIZE(org)-1-1);

	tgilist_free_values(l);
	tgilist_free_items(l);
	return 0;
}

int main() {
	int i = 0;
	tgilist(int, short) *l = tgilist_new();

	assert(tgilist_getsize(l) == 0);
	tgilist_add(l, i);
	assert(tgilist_getsize(l) == 1);
	i++;
	tgilist_add(l, i);
	assert(tgilist_getsize(l) == 2);
	i++;
	tgilist_add(l, i);
	assert(tgilist_getsize(l) == 3);
	i++;

	/* i is 3 at this point */
	tgilist_insert(l, i, 2);
	assert(tgilist_getsize(l) == 4);
	i++;
	assert(tgilist_get(l, 0) == 0);
	assert(tgilist_get(l, 1) == 1);
	assert(tgilist_get(l, 2) == 3);
	assert(tgilist_get(l, 3) == 2);

	assert(tgilist_getsize(l) == 4);

	tgilist_free(l);

	l = tgilist_new();
	assert(tgilist_getsize(l) == 0);

	i = 0;
	for(i=0; i<16384; i++) {
		tgilist_insert(l, i, i);
		assert(tgilist_getsize(l) == i+1);
	}

	assert(tgilist_get(l, 0) == 0);
	assert(tgilist_get(l, 1) == 1);
	assert(tgilist_get(l, 2) == 2);
	assert(tgilist_get(l, 3) == 3);

	assert(tgilist_get(l, 16000) == 16000);

	i = 3;
	assert(tgilist_insert(l, i, 3));
	assert(tgilist_get(l, 3) == 3);
	assert(tgilist_get(l, 4) == 3);

	assert(tgilist_get(l, 16000) == 15999);




	l = tgilist_new();
	assert(tgilist_getsize(l) == 0);

	i = 0;

	tgilist_insert(l, i, 0);
	assert(tgilist_getsize(l) == 1);
	i++;
	tgilist_insert(l, i, 0);
	assert(tgilist_getsize(l) == 2);
	i++;
	tgilist_insert(l, i, 0);
	assert(tgilist_getsize(l) == 3);
	i++;
	tgilist_insert(l, i, 0);
	assert(tgilist_getsize(l) == 4);
	i++;

	assert(tgilist_get(l, 0) == 3);
	assert(tgilist_get(l, 1) == 2);
	assert(tgilist_get(l, 2) == 1);
	assert(tgilist_get(l, 3) == 0);

	return strtest();
}
