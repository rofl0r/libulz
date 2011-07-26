#include "../../include/optparser.h"
#include "../../include/strlib.h"

int op_hasflag(opts* options, char flag) {
	if(!options) return 0;
	return(containsChar(options->flags->ptr, flag));
}
