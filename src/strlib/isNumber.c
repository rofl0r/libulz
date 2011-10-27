#include "../../include/strswitch.h"

int isNumber(char* i) {
	switch(*i) {
		case STRSWITCH_NUMBERS: 
			return 1;
		default:
			return 0;
	}
}
