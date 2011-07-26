#include "../../include/strlib.h"

int containsChar(char* str, char what) {
	char* c = str;
	if(!c) return 0;
	while(*c) {
		if (*c == what) return 1;
		c++;
	}
	return 0;
}
