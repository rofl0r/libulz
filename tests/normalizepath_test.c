#include "../include/filelib.h"
#include "../include/stringptr.h"
#include <stdio.h>
#include <assert.h>

int test(stringptr* x) {
	int res;
	if((res =normalizepath(x)))
		puts(x->ptr);
	return res;
}

int main() {
	stringptr* t1 = stringptr_copy(SPLITERAL("/usr/bin/../local/bin/./gcc/../.."));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/usr/bin/..//.././/./usr/../.."));
	assert(!test(t1));
	t1 = stringptr_copy(SPLITERAL("/usr/bin/..//.././/./usr/../"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/usr/bin/..//.././/.//../.."));
	assert(!test(t1));
	t1 = stringptr_copy(SPLITERAL("/usr/local//bin"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("////usr/local//bin"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/./././usr/local//bin"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/./././usr/local//bin/../../"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/./././usr/local//bin/../../.."));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/usr/.local//bin../.../foo"));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/...../usr/.local//bin../.../foo/."));
	assert(test(t1));
	t1 = stringptr_copy(SPLITERAL("/./...../usr/.local//bin../.../foo/."));
	assert(test(t1));
	
	return 0;
}