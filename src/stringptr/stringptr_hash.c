#include "../../include/stringptr.h"
/* generic string hash algo taken from musl.
 * according to author rich felker, it is released as public domain */
uint32_t stringptr_hash(stringptr* str) {
	uint_fast32_t h = 0;
	unsigned char* s = (void*) str->ptr;
	size_t size = str->size;
	while (size) {
		h = 16*h + *s++;
		h ^= h>>24 & 0xf0;
		size--;
	}
	return h & 0xfffffff;
}
