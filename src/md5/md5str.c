#include "../../include/md5.h"
#include "../../include/strlib.h"

void md5str(char dst[static 1+(MD5_HASH_SIZE*2)], const unsigned char* src, size_t len) {
	md5_ctx m;
	unsigned char raw[MD5_HASH_SIZE];
	md5_init(&m);
	md5_update(&m, src, len);
	md5_end(&m, raw);
	raw2hex(dst, raw, MD5_HASH_SIZE);
}
