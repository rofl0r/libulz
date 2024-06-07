#ifndef FILELIB_H
#define FILELIB_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include "stringptr.h"

char* getFileExt(const char* filename, size_t fnlen);
time_t getFileModTime(const char* filename);
size_t getfilesize(char* filename);
ssize_t getpathdiff(stringptr* path1, stringptr* path2, char* workbuf);
char* ulz_mkdtemp(char* templ);
size_t mktempdir(const char* prefix, char* buffer, size_t bufsize);
char* getfilename(stringptr* path);
int normalizepath(stringptr* path);
/* removes multiple slashes, /./ style stuff and resolves ../ kind of stuff on _absolute_ paths.
 * will always strip trailing slashes.
 * will fail with relative paths. does not resolve symlinks.
 * out must point to a buffer of sufficient size, which equals (worst case) strlen(path)+1.
 * returns 0 on failure, on success out.
*/
char* normalizepath_cstr(const char* path, char* out);

#pragma RcB2 DEP "../src/filelib/*.c"

#endif
