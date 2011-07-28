#include "../../include/stringptr.h"
#include <sys/types.h>

char* stringptr_rchr(stringptr* haystack, int needle) {
	ssize_t i;
	if(!haystack || !haystack->size) return NULL;
	for(i = haystack->size - 1; i >= 0; i--) {
		if(haystack->ptr[i] == needle)
			return haystack->ptr + i;
	}
	return NULL;
}
