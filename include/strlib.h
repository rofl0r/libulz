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
#include <stdarg.h>
#include <unistd.h>

#ifdef __GNUC__
/* format (ARCHETYPE, STRING-INDEX, FIRST-TO-CHECK) 
   ARCHETYPE is printf, scanf, strftime or strfmon
   STRING-INDEX is index of format string (starting with 1)
   FIRST-TO-CHECK is index of first argument to check (-"-)  
   http://ohse.de/uwe/articles/gcc-attributes.html#func-format */
#  define PRINTF_FORMAT1 __attribute__ ((format (printf, 1, 2)))
#  define PRINTF_FORMAT2 __attribute__ ((format (printf, 2, 3)))
#  define PRINTF_FORMAT3 __attribute__ ((format (printf, 3, 4)))
#else
#  define PRINTF_FORMAT1
#  define PRINTF_FORMAT2
#  define PRINTF_FORMAT3
#endif

typedef enum {
	NTS_NONE = 0,
	NTS_PAD = 1,
	NTS_SIGNED_TYPE = 2,
	NTS_LOWERCASE_CHARS = 4
} numberToString_flags;

time_t getFileModTime(const char* filename);
char* getFileExt(const char* filename, size_t fnlen);
int containsChar(char* str, char what);
int hexval(char* i);
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

void ulz_printf(const char* fmt, ...) PRINTF_FORMAT1;
void ulz_fprintf(int fd, const char* fmt, ...) PRINTF_FORMAT2;
ssize_t ulz_snprintf(char* dest, size_t destsize, const char* fmt, ...) PRINTF_FORMAT3;
ssize_t ulz_vsnprintf(char* dest, size_t destsize, const char* format, va_list ap);

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

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/strlib/*.c"
