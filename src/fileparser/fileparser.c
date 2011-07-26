#include "../../include/fileparser.h"
#include <errno.h>

// success: 0
int fileparser_open(fileparser* parser, char* filename) {
	if(!parser || !filename) return -1;
	if(!(parser->file = fopen(filename, "r"))) return errno;
	parser->fpos = 0;
	return 0;
}

// success: 0
int fileparser_readline(fileparser* parser) {
	size_t i;
	size_t found = 0;
	if(!parser || !parser->file) return -1;
	if(!(parser->len = fread(parser->buf, 1, sizeof(parser->buf), parser->file))) return -2;
	for(i = 0; i < parser->len; i++) {
		if(parser->buf[i] == '\n') {
			found = 1;
			nl:
			parser->buf[i] = 0;
			parser->fpos += i + 1;
			parser->len = i;
			if(found && fseek(parser->file, parser->fpos, SEEK_SET) == -1) return errno;
			
			return 0;
		}
	}
	if(parser->len < sizeof(parser->buf)) {
		i = parser->len;
		goto nl;
	}
	return -3;
}

// success: 0
int fileparser_close(fileparser* parser) {
	if(!parser) return -1;
	if(parser->file) fclose(parser->file);
	parser->file = NULL;
	parser->fpos = 0;
	parser->len = 0;
	return 0;
}

// success: 0
int fileparser_getline(fileparser* parser, stringptr* line) {
	if(parser && line) {
		line->ptr = parser->buf;
		line->size = parser->len;
		return 0;
	}
	return -1;
}

