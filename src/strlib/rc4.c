#include <string.h>

#define swap(A, B) { unsigned tmp = A; A = B; B = tmp; }
void rc4(unsigned char* dst, const unsigned char* src, size_t len, const unsigned char* key, size_t keylen)
{
	unsigned char box[256];
	size_t x, y, z;
	for(x=0; x<256; x++) box[x] = x;
	for(x=y=z=0; x<256; x++) {
		y = (y + box[x] + key[z]) % 256;
		swap(box[x], box[y]);
		if(++z == keylen) z=0;
	}
	for(x=y=z=0; x<len; x++) {
		y = (y + 1) % 256;
		z = (z + box[y]) % 256;
		swap(box[y], box[z]);
		dst[x] = src[x] ^ box[(box[y]+box[z]) % 256];
	}
}
