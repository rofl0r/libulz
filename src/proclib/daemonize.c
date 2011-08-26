#include "../../include/logger.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void daemonize(void) {
	pid_t pid = fork();
	if (pid < 0) {
#ifndef NO_DAEMONIZE_OUTPUT
		log_puts(2, SPLITERAL("fork error"));
#endif
		exit(1);
	}
	if (pid > 0) exit(0);
	setsid(); /* obtain a new process group */
	int fd;
	for (fd = getdtablesize(); fd >= 0; --fd) close(fd); /* close all descriptors */
	fd = open("/dev/null", O_RDWR); dup(fd); dup(fd); /* handle standart I/O */
}
