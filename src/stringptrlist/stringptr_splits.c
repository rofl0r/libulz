#include "../../include/stringptrlist.h"

// splits a stringptr into a list
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, the first character of any occurence of delim in the original buffer will be replaced with '\0'
// the count of items will always be number of matches + 1
// i.e. if there's no match, the entire source buffer will be returned as first listitem.
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim) {
	size_t i, linestart = 0;
	stringptrlist* result = stringptrlist_new(16);
	if(!result) return NULL;
	i = 0;
	while(i<=buf->size) {
		if(i == buf->size || stringptr_here(buf, i, delim)) {
			stringptrlist_add(result, buf->ptr + linestart, i - linestart);
			linestart = i + delim->size;
			buf->ptr[i] = 0;
			i += delim->size;
		} else
			i++;
	}
	return result;
}
