/*
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 */

#ifndef _BITARRAY_H_
#define _BITARRAY_H_

#include <stddef.h>

typedef char bitarray;

bitarray* ba_new(size_t size);
void ba_set(bitarray* ba, size_t bit, char value);
char ba_get(bitarray* ba, size_t bit);

#endif

//RcB: DEP "../src/bitarray/bitarray.c"
