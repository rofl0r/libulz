#include "../include/stringptr.h"

int main(void) {
	char* test = "test";
	stringptr tester;
	tester.ptr = test;
	tester.size = 4;
	stringptr_tofile("test.txt", &tester);
	return 0;
}

