#include "../include/sblist.h"
//#include <assert.h>
#include <stdlib.h>

#define assert(X) do { if(!(X)) abort(); } while(0)

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

static inline void test_single(int max, int blocks, int blocktimes) {
	sblist l_l, *l = &l_l;
	int o_0 = 0;
	struct test1* O_o;
	size_t i;
	void* disturber = malloc(1);
	
	o_0 = 0;
	sblist_init(l, sizeof(struct test1), blocks);
	for(i = 0; i < max * blocktimes; i++) {
		int temp = o_0++;
		if(!(i % 4)) {
			free(disturber);
			disturber = malloc((rand() % 256) + 1);
		}
		sblist_add(l, make_struct(temp, o_0++, disturber));
	}
	o_0 = 0;
	for(i = 0; i < max * blocktimes; i++) {
		assert((O_o = (struct test1*) sblist_get(l, o_0/2)) && O_o->x == o_0++ && O_o->y == o_0++);
	}
	assert(!(O_o = (struct test1*) sblist_get(l, o_0/2)));
	sblist_free_items(l);
	free(disturber);
}

static inline void test_each(int max) {
	int round = 1;
	
	test_1:
	
	test_single(max, round, 10);
	
	round++;
	if(round < max) goto test_1;
}


int main(int argc, char** argv) {
	
	int max = atoi(argv[1]);
	if(argc == 2)
		test_each(max);
	else test_single(max, atoi(argv[2]), 1);
	return 0;
}