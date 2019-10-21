#ifndef _CRC32C_H_
#define _CRC32C_H_

#include <stddef.h>
#include <stdint.h>

#define CRC_PREFIX(NAME, FUNC) NAME ## _ ## FUNC

/**
 * CRC32C - 32-bit CRC using the Castagnoli polynomial 0x11EDC6F41.
 * CRC32  - 32-bit CRC using the standard polynomial 0x04C11DB7.
 */

typedef struct CRC32Context {
	uint32_t state;
} CRC32_CTX;

#define CRC32C_CTX CRC32_CTX
/**
 * CRC32[C]_InitTables(void):
 * Initialize tables.
 * Call this once on program startup before any thread that uses CRC starts.
 */
void CRC_PREFIX(CRC32C, InitTables)(void);
void CRC_PREFIX(CRC32, InitTables)(void);

/**
 * CRC32[C]_Init(ctx):
 * Initialize a CRC32C-computing context.  This function can only fail the
 * first time it is called.
 */
int CRC_PREFIX(CRC32C, Init)(CRC32C_CTX *);
int CRC_PREFIX(CRC32, Init)(CRC32_CTX *);

/**
 * CRC32[C]_Update(ctx, buf, len):
 * Feed ${len} bytes from the buffer ${buf} into the CRC32C being computed
 * via the context ${ctx}.
 */
void CRC_PREFIX(CRC32C, Update)(CRC32C_CTX *, const uint8_t *, size_t);
void CRC_PREFIX(CRC32, Update)(CRC32_CTX *, const uint8_t *, size_t);

/**
 * CRC32[C]_Final(cbuf, ctx):
 * Store in ${cbuf} a value such that 1[buf][buf]...[buf][cbuf], where each
 * buffer is interpreted as a bit sequence starting with the least
 * significant bit of the byte in the lowest address, is a product of the
 * Castagnoli polynomial.
 */
void CRC_PREFIX(CRC32C, Final)(uint8_t[4], CRC32C_CTX *);
void CRC_PREFIX(CRC32, Final)(uint8_t[4], CRC32_CTX *);

#undef CRC_PREFIX

#endif /* !_CRC32C_H_ */

