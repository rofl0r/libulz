#include <stdio.h>
#include "stringptr.h"
#include "../include/iniparser.h"

// gcc -Wall -Wextra -g iniparser_test.c stringptr.c iniparser.c -o iniparser_test

int main(void) {
	ini_section sec;
	stringptr result;
	stringptr* ini = stringptr_copy(SPLITERAL("[main]\nfoo=bar\nbar=baz\n[section1]\nbaz=blah\nblah=foo\n"));
	// we need stringptr_copy since stringptr_linesplit modifies the buffer. which would segfault using a literal.
	stringptrlist* lines = stringptr_linesplit(ini);
	
	sec = iniparser_get_section(lines, SPLITERAL("main"));
	iniparser_getvalue(lines, &sec, SPLITERAL("bar"), &result);
	
	printf("[main] bar=%s, size=%d\n", result.ptr, (int) result.size);
	
	
	sec = iniparser_get_section(lines, SPLITERAL("section1"));
	iniparser_getvalue(lines, &sec, SPLITERAL("blah"), &result);
	
	printf("blah=%s, size=%d\n", result.ptr, (int) result.size);
	
	stringptr_free(ini);
	free(lines);
	
	return 0;
}
