//RcB: DEP "../stringptr.c"

#include <assert.h>
#include "../stringptr.h"



int main(void) {
	stringptr* y;
	y = stringptr_replace(copy_string(SPLITERAL("ABAB")), SPLITERAL("B"), SPLITERAL(""));
	assert(streq(y, SPLITERAL("AA")) && y->size == 2);
	free_string(y);

	y = stringptr_replace(copy_string(SPLITERAL("ABAB")), SPLITERAL("A"), SPLITERAL(""));
	assert(streq(y, SPLITERAL("BB")) && y->size == 2);
	free_string(y);
	
	y = stringptr_replace(copy_string(SPLITERAL("ABB")), SPLITERAL("A"), SPLITERAL(""));
	assert(streq(y, SPLITERAL("BB")) && y->size == 2);
	free_string(y);
	
	y = stringptr_replace(copy_string(SPLITERAL("ABBA")), SPLITERAL("A"), SPLITERAL(""));
	assert(streq(y, SPLITERAL("BB")) && y->size == 2);
	free_string(y);

	y = stringptr_replace(copy_string(SPLITERAL("ABBA")), SPLITERAL("BB"), SPLITERAL(""));
	assert(streq(y, SPLITERAL("AA")) && y->size == 2);
	free_string(y);
	
	y = stringptr_replace(copy_string(SPLITERAL("ABBA")), SPLITERAL("BB"), SPLITERAL("CCCC"));
	assert(streq(y, SPLITERAL("ACCCCA")) && y->size == 6);
	free_string(y);
	
	y = stringptr_replace(copy_string(SPLITERAL("ABBA")), SPLITERAL("BBB"), SPLITERAL("CCCC"));
	assert(streq(y, SPLITERAL("ABBA")) && y->size == 4);
	free_string(y);
	
	y = stringptr_replace(copy_string(SPLITERAL("ABBA")), SPLITERAL("A"), SPLITERAL("CCCC"));
	assert(streq(y, SPLITERAL("CCCCBBCCCC")) && y->size == 10);
	free_string(y);
	
	return 0;
}