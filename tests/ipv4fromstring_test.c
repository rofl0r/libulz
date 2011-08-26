#include "../include/strlib.h"
#define assert(X) do { if(!(X)) abort(); } while(0)

int main (void) {
	unsigned char ip[4];
	ipv4fromstring("010.000.000.001", ip);
	assert(ip[0] == 10 && ip[1] == 0 && ip[2] == 0 && ip[3] == 1);
	ipv4fromstring("128.010.020.031", ip);
	assert(ip[0] == 128 && ip[1] == 10 && ip[2] == 20 && ip[3] == 31);
	
	return 0;
}
