#include "../../include/strlib.h"

int isLower(char* i) {
	switch(*i) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			return 1;
		default:
			return 0;
	}
}
