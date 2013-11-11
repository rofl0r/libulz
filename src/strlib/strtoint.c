#include "../../include/strlib.h"

int strtoint(char* str, size_t len) {
	ssize_t i;
	if(!str || !len) return 0;
	ssize_t mul = 1;
	int result = 0;
	for(i = len - 1; i >= 0 + (str[0] == '-'); i--) {
		result += (str[i] - '0') * mul;
		mul *= 10;
	}
	if(str[0] == '-') result *= -1;
	return result;
}
