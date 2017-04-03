/*
 *
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 * 
 */

#ifndef _STRLIB_H_
#define _STRLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef enum {
	NTS_NONE = 0,
	NTS_PAD = 1,
	NTS_SIGNED_TYPE = 2,
	NTS_LOWERCASE_CHARS = 4
} numberToString_flags;

int containsChar(char* str, char what);
int hexval(char* i);
/* converts raw bytes into lowercase hex string.
   dst buffer must be at least (len*2)+1 bytes. */
void raw2hex(char *dst, const unsigned char *src, size_t len);
/* converts hex stream into raw bytes. output buffer must be
   of size (strlen(hex)+1)/2. returns length of out. */
size_t hex2raw(const char *hex, unsigned char* out);
int isAlpha(char* i);
int isNumber(char* i);
int isLetter(char* i);
int isUpper(char* i);
int isLower(char* i);
void makelower(char* i);
void makeupper(char* i);
char* numberToString(uint64_t number, unsigned int base, char* buffer, size_t maxlen, int flags);
char* intToString(int number, char* buffer);
char* uintToString(unsigned int number, char* buffer);
char* int64ToString(int64_t number, char* buffer);
char* uint64ToString(uint64_t number, char* buffer);
int strtoint(char* str, size_t len);
int64_t strtoint64(char* str, size_t len);
char* strdup_n(char* str, size_t len);
char* strstr_uc(char* haystack, char* needle, size_t needlesize);
char* strstar(const char* haystack, const char* needle, size_t needlesize);
char* findword(char* buf, char* word_uc, size_t word_len);

int ipv4fromstring(char* ipstring, unsigned char* fourbytesptr);
void stringfromipv4(unsigned char* ip_buf_4_bytes, char* outbuf_16_bytes);
int isnumericipv4(const char* ipstring);

#define mem_equal(A, B, C) (memcmp((A), (B), (C)) == 0)
#define str_equal(A, B) (strcmp((A), (B)) == 0)
#define strlitlen(A) (sizeof(A) - 1)

extern const char conv_cypher[];
extern const size_t conv_cyper_len;
extern const char base64_tbl[64];

/* calculates number of bytes base64-encoded stream of N bytes will take. */
#define BASE64ENC_BYTES(N) (((N+2)/3)*4)
/* calculates the required output buffer size to base64-encode
 * N bytes (equals BASE64ENC_BYTES + 1 byte for the zero terminator) */
#define BASE64ENC_BUFSIZE(N) (1+BASE64ENC_BYTES(N))
/* calculates number of bytes base64-decoding of a string of N bytes will
   take. */
#define BASE64DEC_BYTES(N) ((N/4)*3)
/* bytes required for base64 decoded output from an input string of length N */
#define BASE64DEC_BUFSIZE(N) (1+BASE64DEC_BYTES(N))

/* base64 encodes count bytes of src into dest. dest must have at least
   BASE64ENC_BUFSIZE(count) available bytes. you need to check the
   dst buffersize yourself. */
void base64enc(char *dst, unsigned const char* src, size_t count);

/* decodes base64-encoded zero-terminated input string `src` into `dst`.
   `dst` *should* have at least BASE64DEC_BUFSIZE(strlen(src)) bytes available.
   buffersize of `dst` is passed using `dst_len`.
   returns size of decoded buffer if successful,
   0 if malformed input, no input, or buffer to small.
   the returned size may be upto 2 bytes smaller than
   BASE64DEC_BYTES(strlen(src)), if the input string ends with 2 padding
   bytes (i.e. "==").
   even though output can be binary, it will be zero-terminated for convenience.
   so the output buffer needs to be 1 bigger than strictly necessary. */
size_t base64dec(unsigned char* dst, const char *src, size_t dst_len);

/* like base64enc, however for zero-terminated input. i.e. not suitable
   for random binary data. */
int base64enc_str(char *dst, unsigned const char* src, size_t dst_len);

void rc4(unsigned char* dst, const unsigned char* src, size_t len, const unsigned char* key, size_t keylen);

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/strlib/*.c"
