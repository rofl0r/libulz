#include "../../include/strlib.h"

int hexval(char* i) {
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
			return 10 + (*i - 'A');
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			return 10 + (*i - 'a');
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
		case '9': 
			return (*i - '0');
		default:
			return 0;
	}
}
