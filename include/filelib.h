#ifndef FILELIB_H
#define FILELIB_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include "stringptr.h"

char* getFileExt(const char* filename, size_t fnlen);
time_t getFileModTime(const char* filename);
size_t getfilesize(char* filename);
int normalizepath(stringptr* path);
ssize_t getpathdiff(stringptr* path1, stringptr* path2, char* workbuf);

#endif

//RcB: DEP "../src/filelib/*.c"
