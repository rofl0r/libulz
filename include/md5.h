#ifndef MD5_H
#define MD5_H
#define MD5_HASH_SIZE 16

#include <stdint.h>
#include <stddef.h>

typedef struct {
	unsigned char buf[64];
	uint64_t count;
	uint32_t val[4];
} md5_ctx;

void md5_init(md5_ctx *cc);
void md5_update(md5_ctx *cc, const void *data, size_t len);
void md5_end(const md5_ctx *cc, void *dst);

//RcB: DEP "../src/md5/md5.c"
#endif

