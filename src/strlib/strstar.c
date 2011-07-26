#include "../../include/strlib.h"

char* strstar(const char* haystack, const char* needle, size_t needlesize) {
	if(!haystack || !needle || !needlesize) return NULL;
	size_t i;
	char* save;
	while(*haystack) {
		save = (char*) haystack;
		for(i = 0; i < needlesize; i++) {
			if(!*haystack) return NULL;
			if(needle[i] != '*' && *(haystack) != needle[i]) goto next;
			haystack++;
		}
		return save;
		next:
		haystack++;
	}
	return NULL;
}
