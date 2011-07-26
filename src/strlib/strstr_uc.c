#include "../../include/strlib.h"

static const char ludiff = 'a' - 'A';

char* strstr_uc(char* haystack, char* needle, size_t needlesize) {
	if(!haystack || !needle || !needlesize) return NULL;
	char diff;
	size_t i;
	char* save;
	while(*haystack) {
		save = haystack;
		for(i = 0; i < needlesize; i++) {
			if(!*haystack) return NULL;
			if(isLower(haystack)) diff = -ludiff;
			else diff = 0;
			if(*(haystack) + diff != needle[i]) goto next;
			haystack++;
		}
		return save;
		next:
		haystack++;
	}
	return NULL;
}
