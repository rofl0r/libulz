#include "../../include/stringptr.h"
#include "../../include/filelib.h"

// expects 2 absolute paths, the second being a child path of the first, i.e. the 2nd being longer.
// normalizes boths paths (possibly modifying them) and copies the difference into workbuf.
// additionally a zero delimiter is placed in workbuf.
// the size of the difference is returned as result, and -1 on failure.
// workbuf should be at least as big as path2, but not longer than PATH_MAX.
// all returned results start with /
ssize_t getpathdiff(stringptr* path1, stringptr* path2, char* workbuf) {
	normalizepath(path1);
	normalizepath(path2);
	// special case : / is the only case where the end of the string is /
	int isRoot = 0;
	if(path1->size == 1) {
		path1->size = 0;
		isRoot = 1;
	}
	ssize_t result = path2->size - path1->size;
	if(result < 0) return -1;
	if(result) {
		if(memcmp(path1->ptr, path2->ptr, path1->size)) return -1;
		memcpy(workbuf, path2->ptr + path1->size, result);
	}
	workbuf[result] = 0;
	if(isRoot) path1->size = 1;
	return result;
}
