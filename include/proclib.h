#ifndef PROCLIB_H
#define PROCLIB_H

/** @file
 * libulz process management
 */

/**
 * background calling process.
 * detaches from foreground terminal.
 */
void daemonize(void);

#include <spawn.h>

struct process {
	pid_t pid;
	int stdinfd;
	int stdoutfd;
	int stderrfd;
	posix_spawn_file_actions_t fa;
};

/**
 * run a process.
 * @param p  pointer to process struct. initialize manually with desired fds
 *  before usage.
 * @param argv  argv parameter to be passed to the executed process.
 *  requires the last element to be a null pointer.
 * @return 0 on success, -1 on error. in error case, you can query errno. */
int process_open(struct process *p, char* const argv[]);

/**
 * terminate a process started with process_open.
 * closes pipes, waits for process to exit, returns exitcode.
 * @param p  process handle which was initialized with process_open.
 * @return process exitcode.
 */
int process_close(struct process *p);

#pragma RcB2 DEP "../src/proclib/*.c"

#endif
