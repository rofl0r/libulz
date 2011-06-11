/*
 *
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 * 
 */

#include "strlib.h"
#include <sys/stat.h>
#include <time.h>

time_t getFileModTime(const char* filename) {
	struct stat st;
	if(!stat(filename, &st))
		return st.st_mtime;
	else return 0;
}

char* getFileExt(const char* filename, size_t fnlen) {
	char* p = (char*) filename + fnlen;
	while(p - 1 > filename && p[-1] != '.') p--;
	if(p > filename) return p;
	else return NULL;
}

int containsChar(char* str, char what) {
	char* c = str;
	if(!c) return 0;
	while(*c) {
		if (*c == what) return 1;
		c++;
	}
	return 0;
}

static const char ludiff = 'a' - 'A';

int isLetter(char* i) {
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			return 1;
		default:
			return 0;
	}
}

int isUpper(char* i) {
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
			return 1;
		default:
			return 0;
	}
}

int isLower(char* i) {
	switch(*i) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			return 1;
		default:
			return 0;
	}
}

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

void makeupper(char* i) {
	switch(*i) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			*i -= ludiff;
		default:
			return;
	}
}