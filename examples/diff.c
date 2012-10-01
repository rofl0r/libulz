#include "../include/stringptr.h"
#include "../include/strlib.h"
#include "../include/logger.h"
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

void diff(stringptr* f1, stringptr* f2) {
	size_t min;
	size_t diffs = 0;
	min = f1->size;
	size_t l;
	char *p1, *p2;
	assert(f1 && f2);
	if (f2->size != min) {
		ulz_printf("sizes differ! %zu, %zu\n", f1->size, f2->size);
		if (f2->size < min) min = f2->size;
	}
	p1 = f1->ptr;
	p2 = f2->ptr;
	while(p1 < f1->ptr + min) {
		if(*p1 != *p2) {
			l = 0;
			while((p1 + (++l)) < (f1->ptr + min) && (*(++p1) != *(++p2)));
			ulz_printf("difference at offset %zu of size %zu\n", ((size_t) (p1 -l - f1->ptr)),  l);
			diffs++;
		}
		p1++;
		p2++;
	}
	ulz_printf("%zu differences in %zu bytes detected.\n", diffs, min);
}


void syntax(void) {
	ulz_printf("syntax: filename1 filename2\n");
	exit(1);
}

int canread(char* fn) {
	return access(fn, R_OK) != -1;
}

int main(int argc, char** argv) {
	if(argc < 3) syntax();
	if(!canread(argv[1]) || !canread(argv[2])) {
		log_perror("can not read");
		return 1;
	}
	stringptr* fc1 = stringptr_fromfile(argv[1]);
	stringptr* fc2 = stringptr_fromfile(argv[2]);
	diff(fc1, fc2);
	stringptr_free(fc1);
	stringptr_free(fc2);
	return 0;
}
