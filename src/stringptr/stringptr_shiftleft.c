#include "../../include/stringptr.h"

int stringptr_shiftleft(stringptr* s, size_t count) {
	if(!s) return 0;
	if(s->size >= count) {
		s->size -= count;
		s->ptr[s->size] = 0;
		return 1;
	}
	return 0;
}
