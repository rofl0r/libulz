#include "../../include/strlib.h"

int ipv4fromstring(char* ipstring, unsigned char* fourbytesptr) {
	char* start = ipstring;
	size_t outbyte = 0;
	while(outbyte < 4) {
		if(*ipstring == '.' || !*ipstring) {
			fourbytesptr[outbyte] = strtoint(start, ipstring - start);
			start = ipstring + 1;
			outbyte++;
		} 
		if(!*ipstring && outbyte < 4) return 0;
		ipstring++;
	}
	return 1;
}
