#include "../include/tglist.h"
//#include <assert.h>
#include <stdlib.h>

#define assert(X) do { if(!(X)) abort(); } while(0)

#ifndef DATASIZE
#define DATASIZE 48
#endif

struct test1 {
	int x;
	int y;
	char data[DATASIZE - 8];
};

static inline struct test1* make_struct(int x, int y, void* d) {
	static struct test1 test;
	test.x = x;
	test.y = y;
	test.data[rand() % sizeof(test.data)] = ((char*)(d))[0];
	return &test;
}

static inline void test_single(int max, int blocktimes) {
	tglist(_, struct test1) l_l, *l = &l_l;
	int i1 = 0;
	struct test1* t1;
	size_t i;
	void* disturber = malloc(1);

	i1 = 0;
	tglist_init(l);
	for(i = 0; i < max * blocktimes; i++) {
		int temp = i1++;
		if(!(i % 4)) {
			free(disturber);
			disturber = malloc((rand() % 256) + 1);
		}
		tglist_add(l, *make_struct(temp, i1++, disturber));
	}
	i1 = 0;
	for(i = 0; i < max * blocktimes; i++) {
		assert((t1 = &tglist_get(l, i1/2)) && t1->x == i1++ && t1->y == i1++);
	}
	// ?? assert(!(t1 = (struct test1*) sblist_get(l, i1/2)));
	tglist_free_items(l);
	free(disturber);
}

int main(int argc, char** argv) {
	int max = argc > 1 ? atoi(argv[1]) : 64*1024;
	test_single(max, 1);
	return 0;
}
