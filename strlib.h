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
#include <time.h>

time_t getFileModTime(const char* filename);
char* getFileExt(const char* filename, size_t fnlen);
int containsChar(char* str, char what);
int isLetter(char* i);
int isUpper(char* i);
int isLower(char* i);
void makelower(char* i);
void makeupper(char* i);

#ifdef __cplusplus
}
#endif

#endif

