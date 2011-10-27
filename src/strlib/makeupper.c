#include "../../include/strswitch.h"

static const char ludiff = 'a' - 'A';

void makeupper(char* i) {
	switch(*i) {
		case STRSWITCH_LETTERS_LC:
			*i -= ludiff;
		default:
			return;
	}
}
