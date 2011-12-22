#include "../../include/strlib.h"

#define LEADING_WHITESPACE ' ': case '\t': case '\n': case ':': case ',': case '.': case ';': case '-'

static inline int isLeadingWhitespace(char c) {
	switch(c) {
		case LEADING_WHITESPACE:
			return 1;
		default:
			return 0;
	}
}

static inline int isTrailingWhitespace(char c) {
	switch(c) {
		case LEADING_WHITESPACE: case '\r':
			return 1;
		default:
			return 0;
	}
}

char* findword(char* buf, char* word_uc, size_t word_len) {
	char* res = buf;
	while(*res && (res = strstr_uc(buf, word_uc, word_len))) {
		if((res == buf || isLeadingWhitespace(res[-1])) &&
		   (res[word_len] == 0 || isTrailingWhitespace(res[word_len])))
			return res;
		else buf = res + 1;
	}
	return NULL;
}
