#include "../../include/fileparser.h"
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// success: 0
int fileparser_open(fileparser* parser, char* filename) {
	if(!parser || !filename) return -1;
	if((parser->file = open(filename, O_RDONLY)) == -1) return errno;
	parser->fpos = 0;
	return 0;
}

// success: 0
// -3: buffer to small
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
	// this covers the case when the file doesnt end with \n
	if(parser->len < bufsize) {
		i = parser->len;
		goto nl;
	}
	// too big for our buffer, reset fp to last position.
	if(lseek(parser->file, (off_t) parser->fpos, SEEK_SET) == -1) return errno;
	return -3;
}
// success: 0
// -3: buffer to small
int fileparser_readline(fileparser* parser) {
	return fileparser_readline_userbuf(parser, parser->buf, sizeof(parser->buf));
}

// returns length of next line, or -1 on failure
ssize_t fileparser_getnextlinelength(fileparser* parser) {
	size_t res = 0;
	uint64_t store_fpos = parser->fpos;
	int ret;
	while((ret = fileparser_readline(parser)) == -3) {
		res += sizeof(parser->buf);
		parser->fpos += sizeof(parser->buf);
		if(lseek(parser->file, (off_t) parser->fpos, SEEK_SET) == -1) return -1;
	}
	if(ret != 0) return -1;
	res += parser->len;
	parser->fpos = store_fpos;
	if(lseek(parser->file, (off_t) parser->fpos, SEEK_SET) == -1) return -1;
	return res;
}

// success: 0
int fileparser_close(fileparser* parser) {
	if(!parser) return -1;
	if(parser->file > 0) close(parser->file);
	parser->file = -1;
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

