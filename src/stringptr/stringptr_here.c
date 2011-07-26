#include "../../include/stringptr.h"
#include <string.h>

int stringptr_here(stringptr* haystack, size_t bufpos, stringptr* needle) {
	if(needle->size <= haystack->size - bufpos) {
		if(!memcmp(needle->ptr, haystack->ptr + bufpos, needle->size))
			return 1;
	}
	return 0;
}
