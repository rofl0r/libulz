#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "../include/stringptr.h"
#include "../include/sha512.h"

#pragma RcB2 CFLAGS "-DTEST_SHA512"

/* compile with rcb --force */

extern int crypto_hash(unsigned char *out, const unsigned char *m,unsigned long long n);

#if 0
void randombytes(unsigned char * out, unsigned long long sz) {
	int fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
	if(fd == -1) exit(1);
	if(read(fd, out, sz) != sz) exit(1);
	close(fd);
}
#endif

/* outbuf must be at least 128+1 bytes */
static void sha512_to_str(const unsigned char hash[64],char outbuf[129]) {
	size_t i;
	for (i = 0; i<64; ++i) {
		outbuf[2 * i] = "0123456789abcdef"[15 & (hash[i] >> 4)];
		outbuf[2 * i + 1] = "0123456789abcdef"[15 & hash[i]];
	}
	outbuf[2 * i] = 0;
}

/* outbuf must be at least 128+1 bytes , returns 1 on success, otherwise 0 */
static int sha512_hash(const char* filename, char *outbuf) {
	int fd;
	sha512_ctx ctx;
	ssize_t nread;
	char buf[4*1024];
	int success = 0;

	fd = open(filename, O_RDONLY);
	if(fd == -1) return 0;
	sha512_init(&ctx);
	while(1) {
		nread = read(fd, buf, sizeof(buf));
		if(nread < 0) goto err;
		else if(nread == 0) break;
		sha512_update(&ctx, (const uint8_t*) buf, nread);
	}
	success = 1;
	unsigned char* hash = sha512_end(&ctx);
	sha512_to_str(hash, outbuf);
	err:
	close(fd);
	return success;
}

static int test() {
	unsigned char buf[8192];
	unsigned char hash1[64], hash2[64], hash3[64];
	size_t i;
	srand(time(0));
	int err = 0;
	for(i=0;i<sizeof buf;i++) {
		buf[i] = rand() & 0xff;
		crypto_hash(hash1, buf, i);
		sha512_ctx ctx;
		sha512_init(&ctx);
		sha512_update(&ctx, buf, i);
		memcpy(hash2, sha512_end(&ctx), 64);

		sha512_init(&ctx);
		size_t n = i;
		unsigned char* p = buf;
		while(n) {
			sha512_update(&ctx, p, n >= 128 ? 128 : n);
			n -= n>=128?128:n;
			p+=128;
		}
		memcpy(hash3, sha512_end(&ctx), 64);
		if(memcmp(hash1, hash2, 64) || memcmp(hash1, hash3, 64)) { err = 1; printf("err at %zu\n", i); }
	}
	return err;
}

static int test_file(char* name) {
	char hash1[64*2+1], hash2[64*2+1];
	unsigned char h[64];
	sha512_hash(name, hash1);
	stringptr* f = stringptr_fromfile(name);
	crypto_hash(h, (void*)f->ptr, f->size);
	sha512_to_str(h, hash2);
	int err = 0;
	if(strcmp(hash1, hash2)) { err = 1; printf("err at file checksum comparison"); }
	else printf("%s\n", hash1);
	return err;
}

int main(int argc, char**argv) {
	if(test()) return 1;
	if(argc == 1) return 0;
	return test_file(argv[1]);
}
