#include "../../include/stringptr.h"

static inline int islb(char* p) {
	return *p == '\n' || *p == '\r';
}

// removes _all_ \r and \n 's from the tail of a stringptr. returns the count of chars removed.
size_t stringptr_chomp(stringptr* s) {
	if(!s) return 0;
	size_t res = 0;
	while(s->size && islb(s->ptr + (s->size - 1)) && stringptr_shiftleft(s, 1)) res++;
	return res;
}
