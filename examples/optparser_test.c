#include "../include/optparser.h"
#include <assert.h>

// laughably the test needs c++0x so the strings can be passed on the stack without too much hardcoding.

//  c++ -Wall -std=c++0x -g optparser_test.c optparser.c stringptr.c strlib.c -o optparser_test
// or

//  c++ -Wall -std=c++0x -g optparser_test.c optparser.c stringptr.c strlib.c -o optparser_test -DMANUAL
//and 
//gdb --args ./optparser_test -cmd=test -cmd2=test2=test2 -cmd -o -p -q -rst


int main(int argl, char** argv) {
	
#define STK __attribute__ ((section ("STACK")))

	char a0[] = "a.out";
	char a1[] = "-cmd=test";
	char a2[] = "-cmd2=test2=test2";
	char a3[] = "-cmd";
	char a4[] = "-o";
	char a5[] = "-p";
	char a6[] = "-q";
	char a7[] = "-rst";


	static char* buf[] STK = {
		a0,
		a1,
		a2,
		a3,
		a4,
		a5,
		a6,
		a7};
#undef STK		
	int argc = 8;
#ifdef MANUAL	
	opts* opt = op_parse(argl, argv);
#else	
	opts* opt = op_parse(argc, buf);
#endif	
	stringptr* temp;
	
	assert(!op_hasflag(opt, 'a'));
	assert(!op_hasflag(opt, 'b'));
	assert(!op_hasflag(opt, 'z'));
	
	assert(op_hasflag(opt, 'o'));
	assert(op_hasflag(opt, 'p'));
	assert(op_hasflag(opt, 'q'));
	
	assert(op_hasflag(opt, 'r'));
	assert(op_hasflag(opt, 's'));
	assert(op_hasflag(opt, 't'));
	
	assert(op_hasflag(opt, 'c'));
	assert(op_hasflag(opt, 'm'));
	assert(op_hasflag(opt, 'd'));
	
	temp = op_get(opt, "cmd");
	assert(temp && !strcmp(temp->ptr, "test"));
	temp = op_get(opt, "cmd2");
	assert(temp && !strcmp(temp->ptr, "test2=test2"));
	assert(!op_get(opt, "cmd3"));
	assert(!op_get(opt, "cmd4"));
	assert(!op_get(opt, "foo"));
	
	op_free(opt);
	
	return 0;
	
}
