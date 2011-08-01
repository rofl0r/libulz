#include "fileparser_internal.h"

int fileparser_readline(fileparser* parser) {
	return fileparser_readline_userbuf(parser, parser->buf, sizeof(parser->buf));
}

