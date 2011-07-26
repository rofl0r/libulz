#include "../../include/optparser.h"
#include "../../include/logger.h"

void op_printall(opts* options) {
	size_t i;
	if (!options || !options->options || !options->options->size) return;
	for(i = 0; i < options->options->size/2; i++)
		log_put(0, VARIS(stringptrlist_get(options->options, (i*2))), VARISL(": "), VARIS(stringptrlist_get(options->options, (i*2 + 1))), NULL);
	if(options->flags && options->flags->size && options->flags->ptr) {
		log_puts(0, options->flags);
		log_putln(0);
	}
}

