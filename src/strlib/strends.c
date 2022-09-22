#include "../../include/strlib.h"
int strends(char* what, char* with) {
	size_t la = strlen(what), li = strlen(with);
	return(la >= li && !strcmp(what+la-li, with));
}

