#include "../../include/strlib.h"

static const char ludiff = 'a' - 'A';

void makelower(char* i) {	
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
			*i += ludiff;
		default:
			return;
	}
}
