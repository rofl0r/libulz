#include <stdarg.h>
#include "../../include/stringptr.h"

// use like this: stringptr_concat(str1, str2, str3, NULL);
// returns a new string made up of all single strings passed...
stringptr* stringptr_concat(stringptr* self, ...) {
	size_t totalsize = self->size;
	stringptr* candidate = NULL;
	stringptr* result = NULL;
	char* dest;

	va_list ap;
	va_start(ap, self);
	while(1) {
		candidate = va_arg(ap, stringptr*);
		if(!candidate) break;
		totalsize += candidate->size;
	}
	va_end(ap);

	if ((result = stringptr_new(totalsize))) {
		memcpy(result->ptr, self->ptr, self->size);
		dest = result->ptr + self->size;
		va_start(ap, self);
		while(1) {
			candidate = va_arg(ap, stringptr*);
			if(!candidate) break;
			memcpy(dest, candidate->ptr, candidate->size);
			dest += candidate->size;
		}
		va_end(ap);
	}
	return result;
}
