#include "../../include/strlib.h"

int isNumber(char* i) {
	switch(*i) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
		case '9': 
			return 1;
		default:
			return 0;
	}
}
