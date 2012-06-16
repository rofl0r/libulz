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

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/strlib/*.c"
