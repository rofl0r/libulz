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

typedef struct {
	unsigned argc;
	char** argv;
} op_state;

void op_init(op_state* p, int argc, char** argv);
char* op_get(op_state* p, stringptr* arg);
int op_hasflag(op_state* p, stringptr* flag);
void op_printall(op_state* p);

#ifdef __cplusplus
}
#endif

#endif

//RcB: DEP "../src/optparser/*.c"
