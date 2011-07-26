#include "../../include/strlib.h"
#include <string.h>
#include <stdlib.h>

char* strdup_n(char* str, size_t len) {
	if (!str || !len) return NULL;
	char * res;
	if((res = malloc(len + 1)))
		memcpy(res, str, len + 1);
	return res;
}

