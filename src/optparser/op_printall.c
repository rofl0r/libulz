#include "../../include/optparser.h"
#include "../../include/logger.h"

void op_printall(op_state* p) {
	size_t i;
	for(i = 1; i < p->argc; i++) {
		log_putc(1, p->argv[i]);
		log_putln(1);
	}
}

