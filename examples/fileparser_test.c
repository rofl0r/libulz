#include "../fileparser.h"
#include <stdio.h>

// the missing newline at EOF is part of the functionality test.

// gcc -Wall -Wextra fileparser_test.c ../fileparser.c -o filep_test

int main(int argc, char** argv) {
	(void) argc; (void) argv;
	fileparser pr;
	fileparser* p = &pr;
	
	fileparser_open(p, __FILE__);
	while(!fileparser_readline(p))
		puts(p->buf);
	
	fileparser_close(p);
	return 0;
}