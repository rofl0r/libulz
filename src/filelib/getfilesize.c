#include <sys/stat.h>
#include <sys/types.h>

#include "../../include/filelib.h"

size_t getfilesize(char* filename) {
	struct stat st;
	if (!stat(filename, &st)) {
		return st.st_size;
	} else return 0;
}

