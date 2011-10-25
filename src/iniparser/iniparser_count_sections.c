#include "../../include/iniparser.h"

static inline int iswhitespace(char* p) {
	return *p == '\t' || *p == ' ';
}

static inline size_t getlastnonwhitespace(stringptr* s) {
	size_t res = s->size;
	while(res && iswhitespace(s->ptr + (res - 1))) res--;
	return res;
}

size_t iniparser_count_sections(stringptrlist* inifile) {
	size_t result = 0;
	stringptr* line;
	size_t i, n;
	for(i = 0; i < stringptrlist_getsize(inifile); i++) {
		line = stringptrlist_get(inifile, i);
		if(!line) continue; // this should never happen...
		if(line->size >= (3) && *line->ptr == '[') {
			n = getlastnonwhitespace(line);
			if(n && line->ptr[n - 1] == ']')
				result++;
		}
	}
	return result;
}
