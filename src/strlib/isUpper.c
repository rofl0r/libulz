#include "../../include/strswitch.h"

int isUpper(char* i) {
	switch(*i) {
		case STRSWITCH_LETTERS_UC:
			return 1;
		default:
			return 0;
	}
}
