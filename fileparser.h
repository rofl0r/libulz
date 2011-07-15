#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "stringptr.h"

typedef struct {
	FILE* file;
	uint64_t fpos;
	size_t len;
	char buf[1024];
} fileparser;

// success: 0
int fileparser_open(fileparser* parser, char* filename);
// success: 0
int fileparser_readline(fileparser* parser);
// success: 0
int fileparser_close(fileparser* parser);
// success: 0
int fileparser_getline(fileparser* parser, stringptr* line);

#endif

