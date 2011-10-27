#include "../../include/strswitch.h"

int isLetter(char* i) {
	switch(*i) {
		case STRSWITCH_LETTERS:
			return 1;
		default:
			return 0;
	}
}
