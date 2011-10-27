#include "../../include/strswitch.h"

int isLower(char* i) {
	switch(*i) {
		case STRSWITCH_LETTERS_LC:
			return 1;
		default:
			return 0;
	}
}
