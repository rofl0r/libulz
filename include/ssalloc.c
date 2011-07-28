/*
 * Single Size Stack Allocator
 * 
 * include this C file directly in your code
 * after defining SSA_MAXELEM and SSA_ELEMSIZE
 * 
 * (C) 2011 rofl0r
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include "bitarray.h"

#ifndef SSA_MAXELEM
#warning setting SSA_MAXELEM programmatically, which is probably not what you want
#define SSA_MAXELEM 1024
#endif
#ifndef SSA_ELEMSIZE
#warning setting SSA_ELEMSIZE programmatically, which is probably not what you want
#define SSA_ELEMSIZE 1024
#endif

typedef struct {
	unsigned char mem[SSA_MAXELEM * SSA_ELEMSIZE];
	unsigned char used[BA_SIZE_REQUIRED(SSA_MAXELEM)];
	size_t top;
	size_t avail;
} ssa_pool;

#define TYPEBITS(X) (sizeof(X) * CHAR_BIT)

#if (SSA_MAXELEM & (-SSA_MAXELEM) != SSA_MAXELEM)
#  error SSA_MAXELEM must be a power of 2
#elif (SSA_MAXLEN > 1024)
#  define SSA_COMPARETYPE uint64_t
#  define SSA_COMPARETYPE_ALL_BITS_SET ((SSA_COMPARETYPE) ~0)
#elif (SSA_MAXLEN <= 128)
#  define SSA_COMPARETYPE uint8_t
#  define SSA_COMPARETYPE_ALL_BITS_SET ((SSA_COMPARETYPE) ~0)
#elif (SSA_MAXLEN <= 512)
#  define SSA_COMPARETYPE uint16_t
#  define SSA_COMPARETYPE_ALL_BITS_SET ((SSA_COMPARETYPE) ~0)
#elif (SSA_MAXLEN <= 1024)
#  define SSA_COMPARETYPE uint32_t
#  define SSA_COMPARETYPE_ALL_BITS_SET ((SSA_COMPARETYPE) ~0)
#else 
#  error BUG !
#endif

static ssa_pool SSA_POOL;

#define SSALLOC(X) ssa_alloc(&SSA_POOL, X)
#define SSFREE(X) ssa_free(&SSA_POOL, X)
#define SSNULL(X) do { SSFREE(X); X = NULL; } while(0);
#define SSINIT ssa_init(&SSA_POOL)

static inline void* ssa_getaddr(ssa_pool* p, size_t elem) {
	return p->mem + ((elem) * SSA_ELEMSIZE);
}

static void* ssa_set(ssa_pool* p, size_t elem) {
	BA_SET_TRUE(p->used, elem);
	p->avail--;
	return ssa_getaddr(p, elem);
}

static ssize_t ssa_getidx(ssa_pool* p, void* ptr) {
	if(((unsigned char*)ptr - p->mem) % SSA_ELEMSIZE) return -1;
	return (((unsigned char*)ptr - p->mem) / SSA_ELEMSIZE);
}

static void* ssa_alloc(ssa_pool* p, size_t size) {
	size_t b, i;
	if(size > SSA_ELEMSIZE) return NULL;
	if(p->top < SSA_MAXELEM) {
		p->top++;
		return ssa_set(p, p->top - 1);
	}
	if(p->avail) {
		for(b = 0; b < SSA_MAXELEM / TYPEBITS(SSA_COMPARETYPE); b++) {
			if(*((SSA_COMPARETYPE*)(p->used + (sizeof(SSA_COMPARETYPE) * b))) != SSA_COMPARETYPE_ALL_BITS_SET)
				for(i = TYPEBITS(SSA_COMPARETYPE) * b; i < TYPEBITS(SSA_COMPARETYPE) * (b + 1); i++)
					if(!BA_GET(p->used, i))
						return ssa_set(p, i);
		}
	}
	return NULL;
}

static void ssa_free(ssa_pool* p, void* ptr) {
	ssize_t idx;
	if((idx = ssa_getidx(p, ptr)) == -1 || idx >= SSA_MAXELEM || !BA_GET(p->used, idx))
#ifdef DEBUG
		abort();
#else
		return;
#endif
	BA_SET_FALSE(p->used, idx);
	p->avail++;
	while(p->top && !BA_GET(p->used, p->top - 1)) p->top--;
}

static void ssa_init(ssa_pool* p) {
	memset(p, 0, sizeof(ssa_pool));
	p->avail = SSA_MAXELEM;
}
