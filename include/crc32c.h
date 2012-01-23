#ifndef _CRC32C_H_
#define _CRC32C_H_

#include <stddef.h>
#include <stdint.h>

/**
 * CRC32C - 32-bit CRC using the Castagnoli polynomial 0x11EDC6F41.
 */

typedef struct CRC32CContext {
	uint32_t state;
} CRC32C_CTX;

/**
 * CRC32C_Init(ctx):
 * Initialize a CRC32C-computing context.  This function can only fail the
 * first time it is called.
 */
int CRC32C_Init(CRC32C_CTX *);

/**
 * CRC32C_Update(ctx, buf, len):
 * Feed ${len} bytes from the buffer ${buf} into the CRC32C being computed
 * via the context ${ctx}.
 */
void CRC32C_Update(CRC32C_CTX *, const uint8_t *, size_t);

/**
 * CRC32C_Final(cbuf, ctx):
 * Store in ${cbuf} a value such that 1[buf][buf]...[buf][cbuf], where each
 * buffer is interpreted as a bit sequence starting with the least
 * significant bit of the byte in the lowest address, is a product of the
 * Castagnoli polynomial.
 */
void CRC32C_Final(uint8_t[4], CRC32C_CTX *);

#endif /* !_CRC32C_H_ */

//RcB: DEP "../src/crc32/crc32c.c"

