#include "stringptr.h"

int main(void) {
	char* test = "test";
	stringptr tester;
	tester.ptr = test;
	tester.size = 4;
	writefile("state.txt", &tester);
	return 0;
}