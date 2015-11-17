#ifndef SHA512_H
#define SHA512_H
#define SHA512_HASH_SIZE 64

#include <stddef.h>

typedef struct sha512_ctx {
	unsigned char h[64], x[256];
	unsigned long long b;
} sha512_ctx;

void sha512_init(sha512_ctx *ctx);
void sha512_update(sha512_ctx *ctx, const unsigned char *buf, size_t n);
/* return value is pointer to the raw hash in a 64 bytes block */
unsigned char* sha512_end(sha512_ctx *ctx);

//RcB: DEP "../src/sha512/sha512.c"
#endif



