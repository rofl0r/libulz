#include "../../include/stringptr.h"
//TODO use read() API because fgets pulls in a whole lot of bloat
#include <stdio.h>

/* supply a buffer long enough to hold the maximum size of an expected input line*/
stringptr read_stdin_line(char* buf, size_t bufsize, int chomp) {
	char* fgets_result;
	static const stringptr nullsp = {NULL, 0};
	if((fgets_result = fgets(buf, bufsize, stdin))) {
		stringptr line;
		stringptr_fromchar(fgets_result, &line);
		if(chomp) 
			stringptr_shiftleft(&line, 1); // remove trailing \n
		return line;
	}
	return nullsp;
}
