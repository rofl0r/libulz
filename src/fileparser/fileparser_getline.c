#include "fileparser_internal.h"

int fileparser_getline(fileparser* parser, stringptr* line) {
	if(parser && line) {
		line->ptr = parser->buf;
		line->size = parser->len;
		return 0;
	}
	return -1;
}

