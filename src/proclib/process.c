#include <spawn.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../include/proclib.h"

extern char** environ;

int process_open(struct process *p, char* const argv[]) {

	errno = posix_spawn_file_actions_init(&p->fa);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_addclose(&p->fa, 0);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_addclose(&p->fa, 1);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_addclose(&p->fa, 2);
	if(errno) return -1;

	int stdin_pipes[2];
	int stdout_pipes[2];
	int stderr_pipes[2];

	if(pipe(stdin_pipes)) goto spawn_error;
	if(pipe(stdout_pipes)) goto spawn_error;
	if(pipe(stderr_pipes)) goto spawn_error;

	p->stdinfd = stdin_pipes[1];
	p->stdoutfd = stdout_pipes[0];
	p->stderrfd = stderr_pipes[0];

	errno = posix_spawn_file_actions_adddup2(&p->fa, stdin_pipes[0], 0);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_adddup2(&p->fa, stdout_pipes[1], 1);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_adddup2(&p->fa, stderr_pipes[1], 2);
	if(errno) goto spawn_error;


	errno = posix_spawn_file_actions_addclose(&p->fa, stdin_pipes[0]);
	if(errno) goto spawn_error;
	errno = posix_spawn_file_actions_addclose(&p->fa, stdin_pipes[1]);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_addclose(&p->fa, stdout_pipes[0]);
	if(errno) goto spawn_error;
	errno = posix_spawn_file_actions_addclose(&p->fa, stdout_pipes[1]);
	if(errno) goto spawn_error;

	errno = posix_spawn_file_actions_addclose(&p->fa, stderr_pipes[0]);
	if(errno) goto spawn_error;
	errno = posix_spawn_file_actions_addclose(&p->fa, stderr_pipes[1]);
	if(errno) goto spawn_error;

	errno = posix_spawnp(&p->pid, argv[0], &p->fa, NULL, argv, environ);
	if(errno) {
		spawn_error:
		posix_spawn_file_actions_destroy(&p->fa);
		return -1;
	}

	close(stdin_pipes[0]);
	close(stdout_pipes[1]);
	close(stderr_pipes[1]);

	return 0;
}

#include <sys/wait.h>
int process_close(struct process *p) {
	close(p->stdinfd);
	close(p->stdoutfd);
	close(p->stderrfd);
	int retval;
	waitpid(p->pid, &retval, 0);
	posix_spawn_file_actions_destroy(&p->fa);
	return WEXITSTATUS(retval);
}

#ifdef TEST
#include <stdio.h>

int main() {
	struct process p;
	int ret = process_open(&p, (char* const[]){"/bin/node", "sha4stdin.js", 0L});
	if(ret) {
		perror("process_open");
		return 1;
	}
	write(p.stdinfd, "foo\n", 4);
	char buf[165];
	read(p.stdoutfd, buf, sizeof buf);
	puts(buf);
	int e = process_close(&p);
	printf("process returned %d status.\n", e);
}
#endif
