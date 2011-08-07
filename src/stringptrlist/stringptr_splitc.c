#include "../../include/stringptrlist.h"

// splits a stringptr into a list
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, delim in the original buffer will be replaced with '\0'
//int stringptrlist_set(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen);
stringptrlist* stringptr_splitc(stringptr* buf, int delim) {
	size_t i, linestart = 0;
	stringptrlist* result = stringptrlist_new(16);
	if(!result) return NULL;

	for(i=0;i<=buf->size;i++) {
		if(i == buf->size || buf->ptr[i] == delim) {
			stringptrlist_add(result, buf->ptr + linestart, i - linestart);
			linestart = i+1;
			buf->ptr[i] = 0;
		}
	}
	return result;
}
