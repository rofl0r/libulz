#include "fileparser_internal.h"

int fileparser_readline_userbuf(fileparser* parser, char* buf, size_t bufsize) {
	size_t i;
	size_t found = 0;
	ssize_t ret;
	if(!parser || !parser->file) return -1;
	if(!(ret = read(parser->file, buf, bufsize))) return -2;
	if(ret == -1) return errno;
	parser->len = (size_t) ret;
	for(i = 0; i < parser->len; i++) {
		if(buf[i] == '\n') {
			found = 1;
			nl:
			buf[i] = 0;
			parser->fpos += i + 1;
			parser->len = i;
			if(found && lseek(parser->file, (off_t) parser->fpos, SEEK_SET) == -1) return errno;
			return 0;
		}
	}
	if(parser->len < bufsize) {
		i = parser->len;
		goto nl;
	}
	if(lseek(parser->file, (off_t) parser->fpos, SEEK_SET) == -1) return errno;
	return -3;
}

