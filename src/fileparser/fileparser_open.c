#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include "fileparser_internal.h"
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

int fileparser_open(fileparser* parser, const char* filename) {
	if(!parser || !filename) return -1;
	if((parser->file = open(filename, O_RDONLY|O_CLOEXEC)) == -1)
		return errno;
	parser->fpos = 0;
	return 0;
}

