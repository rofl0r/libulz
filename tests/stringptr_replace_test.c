#include <assert.h>
#include "../include/stringptr.h"

int main(void) {
	stringptr* y;
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABAB")), SPLITERAL("B"), SPLITERAL(""));
	assert(stringptr_eq(y, SPLITERAL("AA")) && y->size == 2);
	stringptr_free(y);

	y = stringptr_replace(stringptr_copy(SPLITERAL("ABAB")), SPLITERAL("A"), SPLITERAL(""));
	assert(stringptr_eq(y, SPLITERAL("BB")) && y->size == 2);
	stringptr_free(y);
	
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABB")), SPLITERAL("A"), SPLITERAL(""));
	assert(stringptr_eq(y, SPLITERAL("BB")) && y->size == 2);
	stringptr_free(y);
	
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABBA")), SPLITERAL("A"), SPLITERAL(""));
	assert(stringptr_eq(y, SPLITERAL("BB")) && y->size == 2);
	stringptr_free(y);

	y = stringptr_replace(stringptr_copy(SPLITERAL("ABBA")), SPLITERAL("BB"), SPLITERAL(""));
	assert(stringptr_eq(y, SPLITERAL("AA")) && y->size == 2);
	stringptr_free(y);
	
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABBA")), SPLITERAL("BB"), SPLITERAL("CCCC"));
	assert(stringptr_eq(y, SPLITERAL("ACCCCA")) && y->size == 6);
	stringptr_free(y);
	
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABBA")), SPLITERAL("BBB"), SPLITERAL("CCCC"));
	assert(stringptr_eq(y, SPLITERAL("ABBA")) && y->size == 4);
	stringptr_free(y);
	
	y = stringptr_replace(stringptr_copy(SPLITERAL("ABBA")), SPLITERAL("A"), SPLITERAL("CCCC"));
	assert(stringptr_eq(y, SPLITERAL("CCCCBBCCCC")) && y->size == 10);
	stringptr_free(y);
	
	return 0;
}

