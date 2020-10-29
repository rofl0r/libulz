#include "../include/strlib.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

int test(unsigned char *buf, size_t len, const char* key, const char *expected) {
	unsigned char out[2048];
	rc4(out, buf, len, (unsigned char*) key, strlen(key));
	char hex[4096+1];
	raw2hex(hex, out, len);
	return strcmp(hex, expected);
}

#define swap(A, B) { unsigned tmp = A; A = B; B = tmp; }
static void rc4_n(unsigned char* dst, const unsigned char* src, size_t len, const unsigned char* key, size_t keylen, size_t rounds)
{
	unsigned char box[256];
	size_t x, y, z, k;
	for(x=0; x<256; x++) box[x] = x;
	for(k=y=0;k<rounds;++k) {
		for(x=z=0; x<256; x++) {
			y = (y + box[x] + key[z]) % 256;
			swap(box[x], box[y]);
			if(++z == keylen) z=0;
		}
	}
	for(x=y=z=0; x<len; x++) {
		y = (y + 1) % 256;
		z = (z + box[y]) % 256;
		swap(box[y], box[z]);
		dst[x] = src[x] ^ box[(box[y]+box[z]) % 256];
	}
}

static void ciphersaber_2_dec(unsigned char* out, unsigned char *cmsg, size_t clen, char* key, size_t n) {
	unsigned char iv[10];
	assert(clen >= 10);
	memcpy(iv, cmsg, 10);
	size_t l = strlen(key);
	assert(l < 256);
	unsigned char dec_key[ l + 10 ]; /* yay VLA */
	memcpy(dec_key, key, l);
	memcpy(dec_key+l, iv, 10);
	rc4_n(out, cmsg+10, clen-10, dec_key, l+10, n);
}

static void ciphersaber_2_test() {
	/* http://ciphersaber.gurus.org/faq.html#cs2 */
	const char hmsg[] = "ba9ab4cffb7700e618e382e8fcc5ab9813b1abc436ba7d5cdea1a31fb72fb5763c44cfc2ac77afee19ad";
	unsigned char cmsg[(sizeof(hmsg)-1)/2];
	assert(hex2raw(hmsg, cmsg) == sizeof cmsg);
	unsigned char plainmsg[sizeof(cmsg)-10];
	ciphersaber_2_dec(plainmsg, cmsg, sizeof(cmsg), "asdfg", 10);
	assert(!memcmp(plainmsg, "This is a test of CipherSaber-2.", sizeof("This is a test of CipherSaber-2.")-1));
}

static void ciphersaber_1_dec(unsigned char* out, unsigned char *cmsg, size_t clen, char* key) {
	unsigned char iv[10];
	assert(clen >= 10);
	memcpy(iv, cmsg, 10);
	size_t l = strlen(key);
	assert(l < 256);
	unsigned char dec_key[ l + 10 ]; /* yay VLA */
	memcpy(dec_key, key, l);
	memcpy(dec_key+l, iv, 10);
	rc4(out, cmsg+10, clen-10, dec_key, l+10);
}

static void ciphersaber_1_test() {
	/* http://ciphersaber.gurus.org/ */
	const char hmsg[] = "6f6d0babf3aa6719031530edb677ca74e0089dd0e7b8854356bb1448e37cdbefe7f3a84f4f5fb3fd";
	unsigned char cmsg[(sizeof(hmsg)-1)/2];
	assert(hex2raw(hmsg, cmsg) == sizeof cmsg);
	unsigned char plainmsg[sizeof(cmsg)-10];
	ciphersaber_1_dec(plainmsg, cmsg, sizeof(cmsg), "asdfg");
	assert(!memcmp(plainmsg, "This is a test of CipherSaber.", sizeof("This is a test of CipherSaber.")-1));
}

#define TESTA(A, B, C) assert(0 == test(A, sizeof(A)-1, B, C))

int main(int argc, char**argv) {
	unsigned char out[2048], *password="just for test";

	if(argc > 1) {
		rc4(out, argv[1], strlen(argv[1]), password, strlen(password));
		out[strlen(argv[1])]=0;
		int i;
		for(i = 0; i < strlen(argv[1]); i++)
			printf("%c", out[i]);
	} else {
		TESTA("just for testXXXXXXXXXXX", password, "d3c39861ec64855bce92f121713c8ce706f233cdb581b029");
		TESTA("just for testXXXXXXXXXXXsdajdhajskadhsjkaduiiiii3444444444hj99230030jdfkdls nm,mv cxvxc  fsjdljiohiohhfiohweihfphopiwehfifhheihefeiohfewihfeewJJJJJJJJJJJaaaaaaaaaaaaSDDOWERUFDDSSSSSSSSSSSSSSFFFFFFFFFFFFFFFFFFFFFFFFFEEEERTYYYYYYYYYYYRddddddddddUiopooooooooooooSSSSSSYttttt345456737775547754775454754754754745754754754", password, "d3c39861ec64855bce92f121713c8ce706f233cdb581b0293e83b3405fa56ac5c26123486a244d2e0d8e4a0a0fa2e6312fa20f847cd6a5e56985d42c179652e9c6a5fc89395810c9d1a8c2d56c597f576fa2f33fb00abba1a8f101f8299469b2f529b65f087863a238fbcd6b7a767c2b4b5c86632b2a7e9c0e9fea25f55802f65696207fae309221b81b4379055539607b790b9dd1dcefeec2cfec5720e9a3afbbfff8137c0d3070ade5703569483bfd088dbdab9b8de769c786307fd9690f51e1e42f9a4324f0b9aff83f22b229db28847c95622afb2544df4eb097b596d7142873b04d05afd9ffb0006cf12a6d55746b7bf212c248a20aa7325fcae658e3ba364dec9d7b6b38c551ade4a64b07032659ce25d0b4faebc2831ad337ca01323a2c436632ec74e35c3d0d2fc8799da9272ed64a162aa4dd467da84885");
	}
	ciphersaber_1_test();
	ciphersaber_2_test();
	return 0;
}
