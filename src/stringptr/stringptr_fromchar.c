#include "../../include/stringptr.h"
#include <string.h>

stringptr* stringptr_fromchar(char* s, stringptr* out) {
	out->ptr = s;
	out->size = s ? strlen(s) : 0;
	return out;
}
