#include "../../include/strlib.h"


char* intToString(int number, char* buffer, size_t bufsize) {
	if (!buffer || !bufsize) return NULL;
	return numberToString((int64_t) number, 1, 10, buffer, bufsize, 0);
}
