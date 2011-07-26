/*
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 */

#include "bitarray.h"
#include <stdlib.h>

bitarray* ba_new(size_t size) {
	return (bitarray*) calloc((size / 8) + 1, 1);
}

void ba_set(bitarray* ba, size_t bit, char value) {
	char* targetbyte = ba + (bit /8);
	char dist = bit % 8;
	if(value)
		*targetbyte |= (1 << dist);
	else
		*targetbyte &= ~(1 << dist);
}

char ba_get(bitarray* ba, size_t bit) {
	char* targetbyte = ba + (bit /8);
	char dist = bit % 8;
	return *targetbyte & (1 << dist);
}
