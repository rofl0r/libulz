#include "../../include/stringptr.h"
#include "../../include/filelib.h"

// expects 2 absolute paths, the second being a child path of the first, i.e. the 2nd being longer.
// normalizes boths paths (possibly modifying them) and copies the difference into workbuf.
// additionally a zero delimiter is placed in workbuf.
// the size of the difference is returned as result, and -1 on failure.
// workbuf should be at least as big as path2, but not longer than PATH_MAX.
ssize_t getpathdiff(stringptr* path1, stringptr* path2, char* workbuf) {
	normalizepath(path1);
	normalizepath(path2);
	ssize_t result = path2->size - path1->size;
	if(result < 0) return -1;
	if(result) {
		if(memcmp(path1->ptr, path2->ptr, path1->size)) return -1;
		memcpy(workbuf, path2->ptr + path1->size, result);
	}
	workbuf[result] = 0;
	return result;
}