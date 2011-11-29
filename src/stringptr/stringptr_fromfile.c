#include <unistd.h>
#include "../../include/stringptr.h"
#include "../../include/filelib.h"
#include <sys/stat.h>
#include <fcntl.h>

stringptr* stringptr_fromfile(char* filename) {
	int f;
	stringptr* buf;
	size_t size = getfilesize(filename);
	size_t bufpos = 0;
	size_t bytesleft;
	ssize_t bread = 0;
	if(!size) return NULL;
	f = open(filename, O_RDONLY);
	if(f == -1) return NULL;
	buf = stringptr_new(size);
	if (!buf) goto FEXIT;
	
	while(bufpos < size) {
		bytesleft = size - bufpos;
		if((bread = read(f, buf->ptr + bufpos, bytesleft > 64*1024 ? 64*1024 : bytesleft)) < 0) {
			stringptr_free(buf);
			buf = NULL;
			goto FEXIT;
		}
		bufpos += bread;
	}
	FEXIT:
	close(f);
	return buf;
}
