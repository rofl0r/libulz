#include "../../include/optparser.h"

void op_init(op_state* p, int argc, char** argv) {
	p->argc = (unsigned) argc;
	p->argv = argv;
}
