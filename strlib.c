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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

int hexval(char* i) {
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
			return 10 + (*i - 'A');
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
			return 10 + (*i - 'a');
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
		case '9': 
			return (*i - '0');
		default:
			return 0;
	}
}

int isAlpha(char* i) {
	switch(*i) {
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I':
		case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
		case '9': 
			return 1;
		default:
			return 0;
	}
}

int isNumber(char* i) {
	switch(*i) {
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
		case '9': 
			return 1;
		default:
			return 0;
	}
}

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

char* strdup_n(char* str, size_t len) {
	if (!str || !len) return NULL;
	char * res;
	if((res = malloc(len + 1)))
		memcpy(res, str, len + 1);
	return res;
}

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

char* intToString(int number, char* buffer, size_t bufsize) {
	if (!buffer || !bufsize) return NULL;
	return numberToString((int64_t) number, 1, 10, buffer, bufsize, 0);
}

/*
    converts integral type num into a String with base base
    @author rofl0r
    @params num: number to convert
    @params base: i.e. 2 for binary, 10 for dec, 16 for hex
    @params maxLength: signal maximum length of returned string (will be incremented by one if negative, to have space for the "-")
        if 0, the algorithm chooses the maxLength.
		buffer must be able to hold maxlen chars + 1 if signed, + 1 for zero termination.
    @params pad: pad with zeroes
*/
char* numberToString(uint64_t number, int signed_type, unsigned base, char* buffer, size_t maxlen, int pad) {
	static const char conv_cypher[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	static const size_t conv_cyper_len = sizeof(conv_cypher) - 1;
	uint64_t lentest;
	
	size_t tmp, len = 0, i;
	int shr = 0;
	
	if(base % 2 || base > conv_cyper_len) return NULL;
	
	int hasSign = signed_type && number >> 63 == 1;
	if(hasSign) number = ~number + 1;
	
	if(!maxlen) {
		lentest = number;
		while(lentest) {
			len++;
			lentest /= base;
		}
		if(!len) len = 1;
	} else len = maxlen;
	
	tmp = len;
	
	char* result = buffer + hasSign;
	
	for(i = 0; i < len; i++) result[i] = '0';
	result[len] = 0;
	if(!number) {
		if(!pad) result += len - 1;
		return result;
	}

	while (number && len) {
		i = number % base;
		result[len - 1] = conv_cypher[i];
		number -= i;
		len -= 1;
		number /= base;
	}
	if (!pad) {
		for (i = 0; i < tmp; i++) {
			if (result[i] == '0') shr ++;
			else break;
		}
		if (hasSign) shr--;
		result += shr;
	} else if (hasSign)
		result--;
	
	if(hasSign)
		result[0] = '-';

	return result;
}
