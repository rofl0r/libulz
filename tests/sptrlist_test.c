#include "../include/sptrlist.h"
#include <assert.h>

struct test1 {
	int x;
	int y;
};

int main() {
	sptrlist l_l, *l = &l_l;
	int o_0 = 0;
	size_t O_o;
	int round = 1;
	size_t i;
	
	test_1:
	
	o_0 = 1;
	sblist_init(l, sizeof(struct test1), round);
	for(i = 0; i < round * 10; i++) {
		O_o = o_0;
		sptrlist_add(l, O_o);
		o_0++;
	}
	o_0 = 0;
	for(i = 0; i < round * 10; i++) {
		O_o = (size_t) sptrlist_get(l, o_0++);
		assert(O_o == o_0);
	}
	//assert(!sptrlist_get(l, o_0));
	sptrlist_free_items(l);
	
	round++;
	if(round < 10) goto test_1;
	
	return 0;
}