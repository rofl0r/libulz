/*
 * Endianness.h
 *
 * Created on: 29.11.2010
 * 
 * author: rofl0r
 * 
 * License: Public Domain
 * 
 */

#ifndef ENDIANNESS_H_
#define ENDIANNESS_H_

#ifdef __linux
#include <endian.h>
#else
#include <machine/endian.h>
#endif

#if (!defined(__BYTE_ORDER)) && (!defined(BYTE_ORDER))
#error "no __BYTE_ORDER macro found!"
#endif

#ifndef __BYTE_ORDER
#define __BYTE_ORDER BYTE_ORDER
#endif

#if (!defined(__LITTLE_ENDIAN)) && (!defined(LITTLE_ENDIAN))
#error "no __LITTLE_ENDIAN macro found!"
#endif

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __BIG_ENDIAN BIG_ENDIAN
#endif


#if __BYTE_ORDER == __LITTLE_ENDIAN
#define IS_LITTLE_ENDIAN
#endif

#define byteswap16(x) (((x) & 0x00ff) << 8 | ((x) & 0xff00) >> 8)

#define byteswap32(x) \
     ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
      (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

#include <stdint.h>
static inline uint64_t byteswap64(uint64_t x) {
	union { 
		uint64_t ll;
		uint32_t l[2]; 
	} w, r;
	w.ll = x;
	r.l[0] = byteswap32 (w.l[1]);
	r.l[1] = byteswap32 (w.l[0]);
	return r.ll;
}

#ifdef IS_LITTLE_ENDIAN
#define le32(X) (X)
#define le16(X) (X)
#else
#define le32(X) byteswap32(X)
#define le16(X) byteswap16(X)
#endif

#endif /* ENDIANNESS_H_ */
