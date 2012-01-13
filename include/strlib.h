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
ssize_t ulz_vsnprintf(char* dest, size_t destsize, const char* format, va_list ap);
ssize_t ulz_snprintf(char* dest, size_t destsize, const char* fmt, ...);
void ulz_printf(const char* fmt, ...);
void ulz_fprintf(int fd, const char* fmt, ...);
int ipv4fromstring(char* ipstring, unsigned char* fourbytesptr);
void stringfromipv4(unsigned char* ip_buf_4_bytes, char* outbuf_16_bytes);
int isnumericipv4(const char* ipstring);

#define mem_equal(A, B, C) (memcmp((A), (B), (C)) == 0)
#define str_equal(A, B) (strcmp((A), (B)) == 0)
#define strlitlen(A) (sizeof(A) - 1)

extern const char conv_cypher[];
extern const size_t conv_cyper_len;

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/strlib/*.c"
