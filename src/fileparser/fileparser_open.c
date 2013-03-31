#include "fileparser_internal.h"

int fileparser_open(fileparser* parser, const char* filename) {
	if(!parser || !filename) return -1;
	if((parser->file = open(filename, O_RDONLY)) == -1) return errno;
	parser->fpos = 0;
	return 0;
}

