#include "../../include/stringptr.h"
#include <string.h>

stringptr* stringptr_new(size_t size) {
	stringptr* result = (stringptr*) malloc(sizeof(stringptr) + size + 1);
	if (!result) return NULL;
	result->ptr = (char*)result + sizeof(stringptr);
	result->size = size;
	*((char*) (result->ptr + size)) = '\0';
	return result;
}
