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

#include <stddef.h>
#include "stringptr.h"
#include "stringptrlist.h"

typedef struct {
	stringptrlist* options;
	stringptr* flags;	
} opts;

int op_hasflag(opts* options, char flag);
stringptr* op_get(opts* options, const char* optname);
/* IMPORTANT: optparser doesnt alloc any memory for string opts, it simply manipulates the ARGV buffer.
 * this means you dont have to manually free any of the stringptrs you get from op_get */
opts* op_parse(int argc, char** argv);
void op_printall(opts* options);
void op_free(opts* options);

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/optparser/*.c"
