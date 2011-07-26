#include "../../include/stringptr.h"
#include <string.h>

stringptr* stringptr_copy(stringptr* s) {
	if(!s) return NULL;
	stringptr* result = stringptr_new(s->size);
	memcpy(result->ptr, s->ptr, s->size);
	return result;
}
