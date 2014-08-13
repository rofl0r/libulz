#include "../../include/stringptr.h"
#include <string.h>

int stringptr_contains(stringptr* haystack, stringptr* needle) {
	size_t bufpos = 0;
	while(needle->size <= haystack->size - bufpos) {
		if(!memcmp(needle->ptr, haystack->ptr + bufpos, needle->size))
			return 1;
		bufpos++;
	}
	return 0;
}
