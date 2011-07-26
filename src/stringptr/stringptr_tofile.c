#include "../../include/stringptr.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

size_t stringptr_tofile(char* filename, stringptr* buffer) {
	static const size_t chunksize = 64*1024;
	size_t bufpos = 0;
	ssize_t ret = 0;
	int f;
	size_t togo;
	if(!filename || !buffer || !buffer->ptr || (f = open(filename, O_WRONLY | O_CREAT)) == -1) return 0;
	
	while(bufpos < buffer->size) {
		togo = buffer->size - bufpos;
		if((ret = write(f, buffer->ptr+bufpos, togo > chunksize ? chunksize : togo)) < 0) break;
		bufpos += ret;
	}
	close(f);
	return bufpos;
}
