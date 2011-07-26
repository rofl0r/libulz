#include "../../include/filelib.h"
#include <sys/stat.h>

time_t getFileModTime(const char* filename) {
	struct stat st;
	if(!stat(filename, &st))
		return st.st_mtime;
	else return 0;
}
