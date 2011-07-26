#ifndef FILELIB_H
#define FILELIB_H

#include <stddef.h>
#include <time.h>

char* getFileExt(const char* filename, size_t fnlen);
time_t getFileModTime(const char* filename);
size_t getfilesize(char* filename);

#endif

//RcB: DEP "../src/filelib/*.c"
