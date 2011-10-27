#include "../../include/strswitch.h"

static const char ludiff = 'a' - 'A';

void makelower(char* i) {
	switch(*i) {
		case STRSWITCH_LETTERS_UC:
			*i += ludiff;
		default:
			return;
	}
}
