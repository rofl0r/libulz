#include "../../include/optparser.h"

void op_free(opts* options) {
	if (options) {
		free(options->options);
		stringptr_free(options->flags);
		free(options);
	}
}

