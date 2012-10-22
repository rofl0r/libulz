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

#define BYTEM(msb, b6, b5, b4, b3, b2, b1, lsb) ((unsigned char)(lsb | b1 << 1 | b2 << 2 | b3 << 3 | b4 << 4 | b5 << 5 | b6 << 6 | msb << 7))
#define BYTEL(lsb, b1, b2, b3, b4, b5, b6, msb) ((unsigned char)(lsb | b1 << 1 | b2 << 2 | b3 << 3 | b4 << 4 | b5 << 5 | b6 << 6 | msb << 7))

#define BA_SIZE_REQUIRED(X) (((X) / CHAR_BIT) + !!((X) % CHAR_BIT))
#define BA_TARGET_BYTE(X, Y) ((X) + ((Y) / CHAR_BIT))
#define BA_BIT_DISTANCE(X, Y) ((Y) % CHAR_BIT)
#define BA_GET(X, Y) (!!( *BA_TARGET_BYTE(X, Y) & (1 << BA_BIT_DISTANCE(X, Y)) ))
#define BA_GET_SWAPPED(X, Y) BA_GET( (X) , (((Y) / CHAR_BIT) * CHAR_BIT) + ((CHAR_BIT - 1) - ((Y) % CHAR_BIT)) )

#define BA_SET_TRUE(X, Y) do { \
			*(BA_TARGET_BYTE(X, Y)) |= (1 << BA_BIT_DISTANCE(X, Y)); \
			} while(0)
#define BA_SET_FALSE(X, Y) do { \
			*(BA_TARGET_BYTE(X, Y)) &= ~(1 << BA_BIT_DISTANCE(X, Y)); \
			} while(0)
#define BA_SET(BITARR, INDEX, COND) do { if(COND) BA_SET_TRUE(BITARR, INDEX); else BA_SET_FALSE(BITARR, INDEX); } while(0)
			
#endif

