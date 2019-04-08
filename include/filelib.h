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
char* ulz_mkdtemp(char* templ);
size_t mktempdir(const char* prefix, char* buffer, size_t bufsize);
char* getfilename(stringptr* path);

#pragma RcB2 DEP "../src/filelib/*.c"

#endif
