#include "../../include/strswitch.h"

int isAlpha(char* i) {
	switch(*i) {
		case STRSWITCH_ALPHA: 
			return 1;
		default:
			return 0;
	}
}
