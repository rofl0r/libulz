#include "fileparser_internal.h"

int fileparser_close(fileparser* parser) {
	if(!parser) return -1;
	if(parser->file > 0) close(parser->file);
	parser->file = -1;
	parser->fpos = 0;
	parser->len = 0;
	return 0;
}

