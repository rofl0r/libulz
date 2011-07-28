#include "../../include/optparser.h"

int op_hasflag(op_state* p, stringptr* flag) {
	size_t i, l;
	for(i = 1; i < p->argc; i++) {
		if(p->argv[i][0] == '-') {
			l = strlen(p->argv[i] + 1);
			if(l == flag->size && !memcmp(flag->ptr, p->argv[i] + 1, l))
				return 1;
		}
	}
	return 0;
}
