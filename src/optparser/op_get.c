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
	for(i = 1; i < p->argc; i++) {
		if(p->argv[i][0] == '-') {
			l = strleneq(p->argv[i] + 1);
			if(l == arg->size && !memcmp(arg->ptr, p->argv[i] + 1, l))
				return p->argv[i] + 1 + l + 1;
		}
	}
	return NULL;
}
