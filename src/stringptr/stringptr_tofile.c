#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include "../../include/stringptr.h"
#include <unistd.h>
#include <sys/stat.h>
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

size_t stringptr_tofile(char* filename, stringptr* buffer) {
	static const size_t chunksize = 64*1024;
	size_t bufpos = 0;
	ssize_t ret = 0;
	int f;
	size_t togo;
#define OFLAGS (O_CLOEXEC | O_WRONLY | O_CREAT | O_TRUNC)
#define PFLAGS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

	if(!filename || !buffer || !buffer->ptr ||
	   (f = open(filename, OFLAGS, PFLAGS)) == -1) return 0;

	while(bufpos < buffer->size) {
		togo = buffer->size - bufpos;
		if((ret = write(f, buffer->ptr+bufpos,
	            togo > chunksize ? chunksize : togo)) < 0) break;
		bufpos += ret;
	}
	close(f);
	return bufpos;
}
