#include "../../include/filelib.h"

char* getFileExt(const char* filename, size_t fnlen) {
	char* p = (char*) filename + fnlen;
	while(p - 1 > filename && p[-1] != '.') p--;
	if(p > filename) return p;
	else return NULL;
}
