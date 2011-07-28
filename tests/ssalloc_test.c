#define SSA_MAXELEM 256
#define SSA_ELEMSIZE 6
#define DEBUG

#include <assert.h>
#include <stdio.h>
#include "../include/ssalloc.c"
#include <time.h>

int main() {
	size_t i;
	SSINIT;
	char* ptrs[SSA_MAXELEM] = {0};
	srand(time(NULL));
	for(i = 0; i < SSA_MAXELEM; i ++)
		if(rand() % 2 == 1) {
			ptrs[i] = SSALLOC(SSA_ELEMSIZE);
			if(ptrs[i]) memcpy(ptrs[i], "ABCDEF", 6);
		}
		
	for(i = 0; i < SSA_MAXELEM; i ++)
		if(!ptrs[i] && rand() % 2 == 1) {
			ptrs[i] = SSALLOC(SSA_ELEMSIZE);
			if(ptrs[i]) memcpy(ptrs[i], "ABCDEF", 6);
		}
	
	// after 2 rounds 75% should be allocated.
	
	// free some
	for(i = 0; i < SSA_MAXELEM; i ++)
		if(ptrs[i] && rand() % 2 == 1)
			SSNULL(ptrs[i]);
		
	// now we should have some holes
	// fill em all

	for(i = 0; i < SSA_MAXELEM; i ++)
		if(!ptrs[i]) {
			ptrs[i] = SSALLOC(SSA_ELEMSIZE);
			if(ptrs[i]) memcpy(ptrs[i], "ABCDEF", 6);
		}
		
	assert(SSA_POOL.avail == 0);
	assert(SSA_POOL.top == SSA_MAXELEM);
	
	for(i = 0; i < SSA_MAXELEM; i ++) {
		assert(ptrs[i] != NULL);
		assert(!memcmp(ptrs[i], "ABCDEF", 6));
	}

	for(i = 0; i < SSA_MAXELEM; i++) {
		printf("%d\n", i);
		assert(BA_GET(SSA_POOL.used, i) == 1);
	}
	
	
	ssize_t idx = ssa_getidx(&SSA_POOL, ptrs[46]);
	assert(idx >= 0);
	assert(idx < SSA_MAXELEM);
	SSNULL(ptrs[46]);
	assert(SSA_POOL.avail == 1);
	for(i = 0; i < SSA_MAXELEM; i++) {
		printf("%d\n", i);
		if(i != idx)
			assert(BA_GET(SSA_POOL.used, i) == 1);
	}

	for(i = 0; i < SSA_MAXELEM; i ++)
		if(ptrs[i])
			SSNULL(ptrs[i]);
		
	assert(SSA_POOL.avail == SSA_MAXELEM);
	assert(SSA_POOL.top == 0);

	for(i = 0; i < SSA_MAXELEM; i++) {
		printf("%d\n", i);
		assert(BA_GET(SSA_POOL.used, i) == 0);
	}
		
	return 0;
}


