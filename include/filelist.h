#ifndef FILELIST_H
#define FILELIST_H

#undef _GNU_SOURCE
#define _GNU_SOURCE

#include <glob.h>

#include "stringptr.h"
#include "stringptrlist.h"

typedef struct {
	glob_t pglob;
	stringptrlist* files;
} filelist;

typedef enum {
	FLF_NONE = 0,
	FLF_INCLUDE_DOT = 1,
	FLF_INCLUDE_DOT_DOT = 2,
	FLF_EXCLUDE_PATH = 4,
	FLF_INCLUDE_HIDDEN = 8,
} filelistflags;

int filelist_search(filelist* l, stringptr* dir, stringptr* mask, int flags);
void filelist_free(filelist* l);

#pragma RcB2 DEP "../src/filelist/*.c"

#endif

