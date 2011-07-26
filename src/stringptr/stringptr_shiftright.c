#include "../../include/stringptr.h"

int stringptr_shiftright(stringptr* s, size_t count) {
	if(!s) return 0;
	if(s->size >= count) {
		s->size -= count;
		s->ptr += count;
		return 1;
	}
	return 0;
}
