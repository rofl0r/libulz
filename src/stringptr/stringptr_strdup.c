#include <string.h>
#include "../../include/stringptr.h"

char* stringptr_strdup(stringptr* s) {
	if(!s || !s->size) return NULL;
	char* result;
	if(!(result = malloc(s->size + 1))) return NULL;
	memcpy(result, s->ptr, s->size + 1);
	return result;
}

