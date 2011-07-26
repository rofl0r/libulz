#include "../../include/optparser.h"

stringptr* op_get(opts* options, const char* optname) {
	size_t i;
	stringptr s;
	s.ptr = (char*) optname;
	s.size = strlen(optname);
	for(i = 0; i<options->options->size; i+=2) {
		if(stringptr_eq(stringptrlist_get(options->options,i), &s))
			return stringptrlist_get(options->options,i+1);
	}
	return NULL;
}
