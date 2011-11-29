#include <string.h>
#include <stdlib.h>
#include "../../include/stringptr.h"

char* stringptr_strdup(stringptr* s) {
	if(!s) return NULL;
	char* result;
	if(!(result = malloc(s->size + 1))) return NULL;
	memcpy(result, s->ptr, s->size);
	result[s->size] = 0;
	return result;
}

