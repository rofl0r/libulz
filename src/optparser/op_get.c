#include "../../include/optparser.h"
#include <string.h>

static size_t strleneq(const char* str) {
	size_t l = 0;
	while(*str && *str != '=') {
		str++;
		l++;
	}
	if(!*str) return 0;
	return l;
}

char* op_get(op_state* p, stringptr* arg) {
	size_t i, l;
	char *s;
	for(i = 1; i < p->argc; i++) {
		s = p->argv[i];
		if(*s == '-') {
			s++;
			if(arg->size > 1 && *s == '-') s++;
			l = strleneq(s);
			if(l == arg->size && !memcmp(arg->ptr, s, l))
				return s + l + 1;
		}
	}
	return NULL;
}
