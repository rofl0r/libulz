#include "../include/guard_alloc.c"

typedef void (*testfunc)(char*);

static void test_ok1(char* mem) {
	mem[79] = 'a';
}

static void test_ok2(char* mem) {
	mem[PAGE_SIZE*3+79] = 0;
}

static void test_crash(char *mem) {
	mem[PAGE_SIZE*3+80] = 0;
}

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <assert.h>

static int runtest(char* mem, testfunc f) {
	pid_t child, ret;
	int stat_loc;
	if((child = fork()) == 0) {
		f(mem);
		_exit(0);
	} else {
		ret = waitpid(child, &stat_loc, 0);
		assert(ret == child);
		return WIFEXITED(stat_loc) ? 0 : WTERMSIG(stat_loc);
	}
}

int main() {

	char *mem;
	mem = guard_alloc(80);
	assert(runtest(mem, test_ok1) == 0);
	guard_free(mem, 80);

	mem = guard_alloc(PAGE_SIZE*3+80);
	assert(runtest(mem, test_ok2) == 0);

	/* this should crash */
	assert(runtest(mem, test_crash) == SIGSEGV);

	guard_free(mem, PAGE_SIZE*3+80);
	return 0;
}
