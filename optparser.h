/*
 *
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 * simple OptParser for command line options.
 * processes flag type options like -l -t -p etc, either in this single opt form or in a -ltp form
 * also named arguments, in the form of arg=somthing
 * 
 */

#ifndef _OPTPARSER_H_
#define _OPTPARSER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "stringptr.h"
#include "stringptrlist.h"

typedef struct {
	stringptrlist* options;
	stringptr* flags;	
} opts;

int op_hasFlag(opts* options, char flag);
stringptr* op_getOpt(opts* options, const char* optname);
opts* op_parseOpts(int argc, char** argv);
void op_printAll(opts* options);
void op_freeOpts(opts* options);

#ifdef __cplusplus
}
#endif

#endif
