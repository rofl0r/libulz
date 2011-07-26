#include "../../include/stringptrlist.h"

// splits a stringptr into a list
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, delim in the original buffer will be replaced with '\0'
stringptrlist* stringptr_splitc(stringptr* buf, int delim) {
	size_t vol = (4096 - sizeof(stringptrlist)) / sizeof(stringptr); // lets start with one heap page
	size_t i, linestart;
	linestart = 0;
	stringptrlist* result = stringptrlist_new(vol);
	if(!result) return NULL;

	for(i=0;i<=buf->size;i++) {
		if(i == buf->size || buf->ptr[i] == delim) {			
			result->size++;
			if(result->size > vol) {
				vol *= 2;
				result = stringptrlist_resize(result, vol);
				if(!result) return NULL;
			}
			stringptrlist_set(result, result->size-1, buf->ptr + linestart, i - linestart);
			linestart = i+1;
			buf->ptr[i] = 0;
		}
	}
	return result;
}
