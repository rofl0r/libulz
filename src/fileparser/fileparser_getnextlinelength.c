#include "fileparser_internal.h"

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

