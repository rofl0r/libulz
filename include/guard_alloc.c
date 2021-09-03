/*
  guard alloc - allocates memory immediately followed by a guard page.
  this turns every* OOB read/write into a segfault.
  i.e. poor man's asan.
  * - of couse won't catch access that go OOB beyond PAGE_SIZE.
*/

#include <sys/mman.h>
#include <limits.h>
#include <stdint.h>

/* init this once as it's nowadays a func call */
static const size_t guard_alloc_pagesize = PAGE_SIZE;

static void *guard_alloc(size_t n) {
	size_t ps = guard_alloc_pagesize, pages = 1+n/ps;
	char *mem = mmap(0, (pages+1)*ps, PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(mem == MAP_FAILED) return 0;
	if(mprotect(mem + pages*ps, ps, PROT_NONE)) {
		munmap(mem, (pages+1)*ps);
		return 0;
	}
	return mem + pages*ps - n;
}

/* unlike regular free, you need to bookkeep the size originally used */
static void guard_free(void* p, size_t n) {
	size_t off = -n & (guard_alloc_pagesize-1);
	char *mem = (void*)((uintptr_t)p - off);
	munmap(mem, n + off + guard_alloc_pagesize);
}
