#pragma RcB2 CPPFLAGS "-DTLIST_TEST"
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include "../include/tlist.h"
#include "../include/sblist.h"

#define N 20480
#define USE_S 1
#define USE_T 2
int main(int argc, char** argv) {
	int mode;
	if(argc <= 1) mode = USE_S | USE_T;
	else mode = atoi(argv[1]);
	srand(time(0));
	int rnd;
	sblist* l = sblist_new(sizeof(int), 32);
	tlist* t = tlist_new(sizeof(int));
	for(int i = 0; i < N/2; ++i) {
		if(mode & USE_S) sblist_add(l, &i);
		if(mode & USE_T) tlist_append(t, &i);
	}
	for(int i = 0; i < N/2; ++i) {
		rnd = rand();
		if(mode & USE_S) sblist_insert(l, &i, sblist_getsize(l) == 0 ? 0 : rnd%sblist_getsize(l));
		if(mode & USE_T) tlist_insert(t, tlist_getsize(t) == 0 ? 0 : rnd%tlist_getsize(t), &i);
	}
	if(mode == (USE_S|USE_T))
		assert(sblist_getsize(l) == tlist_getsize(t));
	if(mode == (USE_S|USE_T))
	for(int i = 0; i < sblist_getsize(l); ++i) {
		assert(*(int*)tlist_get(t, i) == *(int*)sblist_get(l, i));
	}
	for(int i = 0; i < N/4; ++i) {
		rnd = rand();
		if(mode == (USE_S|USE_T))
			assert(sblist_getsize(l) == tlist_getsize(t));
		if(mode & USE_S)
			sblist_delete(l, sblist_getsize(l) == 0 ? 0 : rnd%sblist_getsize(l));
		if(mode & USE_T)
			tlist_delete(t, tlist_getsize(t) == 0 ? 0 : rnd%tlist_getsize(t));
	}
	for(int i = 0; i < N/8; ++i) {
		rnd = rand();
		if(mode & USE_S) sblist_insert(l, &i, sblist_getsize(l) == 0 ? 0 : rnd%sblist_getsize(l));
		if(mode & USE_T) tlist_insert(t, tlist_getsize(t) == 0 ? 0 : rnd%tlist_getsize(t), &i);
	}
	if(mode == (USE_S|USE_T))
	for(int i = 0; i < sblist_getsize(l); ++i) {
		assert(*(int*)tlist_get(t, i) == *(int*)sblist_get(l, i));
	}
	if(mode & USE_T) {
		printf("balance: %.4f%%\n", tlist_getbalance(t));
		tlist_free(t);
	}
	return 0;
}
