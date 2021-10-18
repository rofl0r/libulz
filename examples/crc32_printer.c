#include "../include/crc32.h"
#include "../include/crc32c.h"

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

typedef int (ifn)(void*);
typedef void (ufn)(void*, const uint8_t *, size_t);
typedef void (ffn)(uint8_t *, void*);

/* crc32 algo as used in R4 DS flash cartridge usrcheat.dat.
   it's actually known as CRC-32/JAMCRC. only first 512bytes
   of the ROM are checked. */

static int crc32_r4_init(uint32_t *ctx) {
	*ctx = -1u;
	return 0;
}

static void crc32_r4_update(uint32_t *ctx, const uint8_t *p, size_t len) {
#define CRCPOLY 0xedb88320
	unsigned i;
	uint32_t crc = *ctx;
	while(len--)
	{
		crc ^= *p++;
		for(i=0; i<8; ++i)
			crc=(crc>>1)^((crc&1)?CRCPOLY:0);
	}
	*ctx = crc;
}

static void crc32_r4_final(uint8_t *out, uint32_t *ctx) {
	union {
		uint8_t b[4];
		uint32_t u;
	} *res = (void*) out;
	res->u = *ctx;
}

static uint32_t selftest(ifn* init, ufn* feed, ffn *final)
{
	CRC32_CTX ctx;
	init(&ctx);
	feed(&ctx, "123456789", 9);
	union {
		uint8_t b[4];
		uint32_t u;
	} res;
	final(res.b, &ctx);
	return res.u;
}

int main(int argc, char **argv) {

	CRC32C_CTX ctxc;
	CRC32_CTX ctx;
	uint32_t r4ctx;

	CRC32C_InitTables();
	CRC32_InitTables();

	/* https://reveng.sourceforge.io/crc-catalogue/17plus.htm#crc.cat-bits.32  */
	uint32_t check;
	/* CRC-32/ISO-HDLC */
	check = selftest(CRC32_Init, CRC32_Update, CRC32_Final);
	assert(check == 0xcbf43926);
	check = selftest(CRC32C_Init, CRC32C_Update, CRC32C_Final);
	assert(check == 0xe3069283);
	check = selftest(crc32_r4_init, crc32_r4_update, crc32_r4_final);
	assert(check == 0x340bc6d9);

	if(argc < 2) {
		printf("usage: %s FILE [N]\n"
		"N is optional and denotes how many bytes to read (e.g. 512 bytes for NDS rom) - max 64K.\n",
		argv[0]);
		return 1;
	}
	int cnt = -1;
	if(argc == 3) cnt = atoi(argv[2]);


	CRC32C_Init(&ctxc);
	CRC32_Init(&ctx);
	crc32_r4_init(&r4ctx);

	FILE *f = fopen(argv[1], "r");
	unsigned char buf[64*1024];
	while(1) {
		size_t n = fread(buf, 1, cnt != -1 ? cnt : sizeof buf, f);
		if(!n) break;
		CRC32C_Update(&ctxc, buf, n);
		CRC32_Update(&ctx, buf, n);
		crc32_r4_update(&r4ctx, buf, n);
		if(cnt != -1) break;
	}
	fclose(f);

	unsigned char crc32c[4], crc32[4], crc32r4[4];

	CRC32C_Final(crc32c, &ctxc);
	CRC32_Final(crc32, &ctx);
	crc32_r4_final(crc32r4, &r4ctx);

	printf("CRC (castagnoli): %02x%02x%02x%02x\n", crc32c[0], crc32c[1], crc32c[2], crc32c[3]);
	printf("CRC (standard)  : %02x%02x%02x%02x\n", crc32[0], crc32[1], crc32[2], crc32[3]);
	printf("CRC (r4/jamcrc) : %02x%02x%02x%02x\n", crc32r4[0], crc32r4[1], crc32r4[2], crc32r4[3]);
}
