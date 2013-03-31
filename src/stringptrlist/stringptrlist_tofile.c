#include "../../include/stringptrlist.h"
#include <fcntl.h>

/* writes a stringptrlist to a file, inserting a newline char after each list entry.
 * returns 1 on success, 0 on error. */
int stringptrlist_tofile(stringptrlist* l, char* filename, int mode) {
	int success = 0;
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if(fd == -1) goto ret;
	stringptr *s;
	sblist_iter(l, s) {
		if(write(fd, s->ptr, s->size) != s->size) goto err;
		if(write(fd, "\n", 1) != 1) goto err;
	}
	success = 1;
	err:
	close(fd);
	ret:
	return success;
}
