#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "stringptr.h"

typedef struct {
	uint64_t fpos;
	size_t len;
	char buf[1024];
	int file;
} fileparser;

// success: 0
int fileparser_open(fileparser* parser, char* filename);
// success:0, buffer to small: -3
int fileparser_readline(fileparser* parser);
// success: 0
int fileparser_close(fileparser* parser);
// success: 0
int fileparser_getline(fileparser* parser, stringptr* line);
// success: size till next nl
ssize_t fileparser_getnextlinelength(fileparser* parser);
// success:0, buffer to small: -3
int fileparser_readline_userbuf(fileparser* parser, char* buf, size_t bufsize);

#endif

//RcB: DEP "../src/fileparser/*.c"
