#include "../../include/stringptr.h"
/* assumes path does not end in '/' */
char* getfilename(stringptr* path) {
	char* result = stringptr_rchr(path, '/');
	if(result) result++;
	return result;
}
