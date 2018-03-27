#include "../../include/optparser.h"

int op_hasflag(op_state* p, stringptr* flag) {
	size_t i, l;
	char* s;
	for(i = 1; i < p->argc; i++) {
		s = p->argv[i];
		if(*s == '-') {
			s++;
			if(flag->size > 1 && *s == '-') s++;
			l = strlen(s);
			if(l == flag->size && !memcmp(flag->ptr, s, l))
				return 1;
		}
	}
	return 0;
}
