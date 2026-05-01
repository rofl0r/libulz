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
	int fds[3];
	posix_spawn_file_actions_t fa;
};

/**
 * run a process.
 *
 * the process will be launched in the background, and can be communicated
 * with using the filedescriptors in the fds array, where indices 0,1,2
 * correspond to stdin,stdout,stderr respectively.
 * the pid member of the struct may also be used with other libc functions
 * taking a pid_t argument.
 *
 * if a started process exits before process_close is called on it,
 * you will receive a SIGPIPE when you try to read/write from one of its fds.
 * this may be undesirable as it tears down your program unless handled.
 * a good strategy is to ignore the signal like so: signal(SIGPIPE, SIG_IGN);
 * or even better, pthread_sigmask - since that also affects multi-threaded
 * processes - and then check for EPIPE on poll or a 0 return value from
 * read/write to determine whether the process exited prematurely.
 * you can then call process_close() and determine the exact reason via the
 * value returned.
 *
 * @param p  pointer to process struct.
 *  use members fds[0-2] to access stdin, stdout, and stderr of the process,
 *  after successful start. (via write/read).
 * @param argv  argv parameter to be passed to the executed process.
 *  requires the last element to be a null pointer.
 * @return 0 on success, -1 on error.
 *  in error case, you can query errno, and there's no need to call
 *  process_close().
 * @see process_close
*/
int process_open(struct process *p, char* const argv[]);

/**
 * terminate a process started with process_open.
 * closes pipes, waits for process to exit, returns waitpid code.
 * @param p  process handle which was initialized with process_open.
 * @return waitpid code.
 * this code is more useful than pure exit code, as it delivers additional
 * information such as whether a process was terminated by a signal, stopped,
 * etc. to get the regular exit code, use the macro WEXITSTATUS from sys/wait.h
 * e.g. if(WIFEXITED(ret)) exitcode = WEXITSTATUS(ret);
 * or   if(WIFSIGNALED(ret)) signal = WTERMSIG(ret);
 */
int process_close(struct process *p);

#pragma RcB2 DEP "../src/proclib/*.c"

#endif
