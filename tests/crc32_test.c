#ifdef  CRC32_CASTAGNOLI
#include "../include/crc32c.h"
#define CRC_PREFIX(FUNC) CRC32C_ ## FUNC
#else
#include "../include/crc32.h"
#define CRC_PREFIX(FUNC) CRC32_ ## FUNC
#endif

#define TEST_STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"


#include <stdio.h>
#include <assert.h>

int main() {
	CRC32_CTX ctx;
	CRC_PREFIX(InitTables)();
	CRC_PREFIX(Init)(&ctx);
	CRC_PREFIX(Update)(&ctx, TEST_STRING, sizeof(TEST_STRING)-1);
	union { unsigned char ch[4]; unsigned val; } val;
	CRC_PREFIX(Final)(val.ch, &ctx);
	printf("result %08x\n", val.val);
#ifdef CRC32_CASTAGNOLI
	assert(val.val == 0xe3d616ff);
#else
	assert(val.val == 0xDBD96D6E);
#endif
}
