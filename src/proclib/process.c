#include <spawn.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../include/proclib.h"

int process_open(struct process *p, char* const argv[]) {
	extern char** environ;
	int pipes[3][2] = {0}, i,j;

	errno = posix_spawn_file_actions_init(&p->fa);
	if(errno) goto spawn_error;

	for(i=0; i<3; ++i) {
		errno = posix_spawn_file_actions_addclose(&p->fa, i);
		if(errno) goto spawn_error;
	}

	for(i=0; i<3; ++i) if(pipe(pipes[i])) goto spawn_error;

	static const unsigned char pipeends[3] = {0,1,1};

	for(i=0; i<3; ++i) p->fds[i] = pipes[i][!pipeends[i]];

	for(i=0; i<3; ++i) {
		errno = posix_spawn_file_actions_adddup2(&p->fa, pipes[i][pipeends[i]], i);
		if(errno) goto spawn_error;
	}
	for(i=0; i<3; ++i) for(j=0; j<2; ++j) {
		errno = posix_spawn_file_actions_addclose(&p->fa, pipes[i][j]);
		if(errno) goto spawn_error;
	}

	errno = posix_spawnp(&p->pid, argv[0], &p->fa, NULL, argv, environ);
	if(errno) {
		spawn_error:
		for(i=0; i<3; ++i) for(j=0; j<2; ++j)
			if(pipes[i][j]) close(pipes[i][j]);
		posix_spawn_file_actions_destroy(&p->fa);
		return -1;
	}

	for(i=0; i<3; ++i)
		close(pipes[i][pipeends[i]]);

	return 0;
}

#include <sys/wait.h>
int process_close(struct process *p) {
	int i, retval;
	for(i=0; i<3; ++i) close(p->fds[i]);
	waitpid(p->pid, &retval, 0);
	posix_spawn_file_actions_destroy(&p->fa);
	return WEXITSTATUS(retval);
}

#ifdef TEST
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main() {
	struct process p;
	int ret = process_open(&p, (char* const[]){"/usr/bin/awk", "-v", "a=b", "{print a $0;}", 0L});
	if(ret) {
		perror("process_open");
		return 1;
	}
	write(p.fds[0], "foo\n", 4);
	char buf[128];
	read(p.fds[1], buf, sizeof buf);
	puts(buf);
	assert(!strcmp(buf, "bfoo\n"));
	int e = process_close(&p);
	printf("process returned %d status.\n", e);
}
#endif
