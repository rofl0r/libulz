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
#include <limits.h>

#define BA_SIZE_REQUIRED(X) (((X) / CHAR_BIT) + !!((X) % CHAR_BIT))
#define BA_TARGET_BYTE(X, Y) ((X) + ((Y) / CHAR_BIT))
#define BA_BIT_DISTANCE(X, Y) ((Y) % CHAR_BIT)
#define BA_GET(X, Y) (!!( *BA_TARGET_BYTE(X, Y) & (1 << BA_BIT_DISTANCE(X, Y)) ))
#define BA_SET_TRUE(X, Y) do { \
			*(BA_TARGET_BYTE(X, Y)) |= (1 << BA_BIT_DISTANCE(X, Y)); \
			} while(0);
#define BA_SET_FALSE(X, Y) do { \
			*(BA_TARGET_BYTE(X, Y)) &= ~(1 << BA_BIT_DISTANCE(X, Y)); \
			} while(0);
			
#endif

