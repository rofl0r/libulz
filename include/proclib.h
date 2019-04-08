#ifndef PROCLIB_H
#define PROCLIB_H

void daemonize(void);

#include <spawn.h>

struct process {
	pid_t pid;
	int stdinfd;
	int stdoutfd;
	int stderrfd;
	posix_spawn_file_actions_t fa;
};

/* return 0 on success, -1 on error. in error case, you can query errno. */
int process_open(struct process *p, char* const argv[]);
/* closes pipes, waits for process to exit, returns exitcode */
int process_close(struct process *p);

#pragma RcB2 DEP "../src/proclib/*.c"

#endif
