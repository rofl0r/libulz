#include "../../include/strlib.h"

/*
    converts integral type num into a String with base base
    @author rofl0r
    @params num: number to convert
    @params base: i.e. 2 for binary, 10 for dec, 16 for hex
    @params maxLength: signal maximum length of returned string (will be incremented by one if negative, to have space for the "-")
        if 0, the algorithm chooses the maxLength.
		buffer must be able to hold maxlen chars + 1 if signed, + 1 for zero termination.
    @params pad: pad with zeroes
*/
char* numberToString(uint64_t number, unsigned base, char* buffer, size_t maxlen, int flags) {
	uint64_t lentest;
	
	size_t tmp, len = 0, i;
	int shr = 0;
	
	if(base % 2 || base > conv_cyper_len) return NULL;
	
	int hasSign = (flags & NTS_SIGNED_TYPE) && number >> 63 == 1;
	if(hasSign) number = ~number + 1;
	
	if(!maxlen) {
		lentest = number;
		while(lentest) {
			len++;
			lentest /= base;
		}
		if(!len) len = 1;
	} else len = maxlen;
	
	tmp = len;
	
	char* result = buffer + hasSign;
	
	for(i = 0; i < len; i++) result[i] = '0';
	result[len] = 0;
	if(!number) {
		if(!flags & NTS_PAD) result += len - 1;
		return result;
	}

	while (number && len) {
		i = number % base;
		if(!((flags & NTS_LOWERCASE_CHARS) && base <= 36 && i >= 10 && i < 36))
			result[len - 1] = conv_cypher[i];
		else 
			result[len - 1] = conv_cypher[i + 26];
			
		number -= i;
		len -= 1;
		number /= base;
	}
	if (!flags & NTS_PAD) {
		for (i = 0; i < tmp; i++) {
			if (result[i] == '0') shr ++;
			else break;
		}
		if (hasSign) shr--;
		result += shr;
	} else if (hasSign)
		result--;
	
	if(hasSign)
		result[0] = '-';

	return result;
}
