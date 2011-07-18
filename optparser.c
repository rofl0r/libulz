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

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "optparser.h"
#include "strlib.h"
#include "logger.h"

//RcB: DEP "stringptr.c"
//RcB: DEP "stringptrlist.c"
//RcB: DEP "strlib.c"
//RcB: DEP "logger.c"


/* IMPORTANT: optparser doesnt alloc any memory for string opts, it simply manipulates the ARGV buffer.
 * this means you dont have to manually free any of the stringptrs you get from op_getOpt
 */

int op_hasFlag(opts* options, char flag) {
	if(!options) return 0;
	return(containsChar(options->flags->ptr, flag));
}

stringptr* op_getOpt(opts* options, const char* optname) {
	size_t i;
	stringptr s;
	s.ptr = (char*) optname;
	s.size = strlen(optname);
	for(i = 0; i<options->options->size; i+=2) {
		if(streq(getlistitem(options->options,i), &s))
			return getlistitem(options->options,i+1);
	}
	return NULL;
}

opts* op_parseOpts(int argc, char** argv) {
	int fc = 0;
	int oc = 0;
	size_t l = 0;
	int i;
	opts* result;
	char* op;
	char* np;
	char* fp;
	size_t lcur = 0;
	// first pass: collect information for later malloc.
	for(i=1;i<argc;i++) {
		if(*argv[i] == '-') {
			if(containsChar(argv[i], '=')) oc++;
			else
				fc+=strlen(argv[i])-1;
		}
	}
	result = (opts*) malloc(sizeof(opts));
	result->flags = new_string(fc);
	result->options = new_stringptrlist(oc * 2);
	result->options->size = oc*2;
	fp = result->flags->ptr;
	// 2nd pass: write struct
	for(i=1;i<argc;i++) {
		op = argv[i];
		if(*op == '-') {
			op++;
			if((np = strchr(op, '='))) {
				setlistitem(result->options,lcur++,op, np-op);
				*np = '\0';
				np++;
				setlistitem(result->options,lcur++,np, strlen(np));
			} else {
				l = strlen(op);
				memcpy(fp, op, l);
				fp += l;
			}
		}
	}
	return result;
}

void op_printAll(opts* options) {
	size_t i;
	if (!options || !options->options || !options->options->size) return;
	for(i = 0; i < options->options->size/2; i++)
		log_put(0, VARIS(getlistitem(options->options, (i*2))), VARISL(": "), VARIS(getlistitem(options->options, (i*2 + 1))), 0);
	if(options->flags && options->flags->size && options->flags->ptr) {
		log_puts(0, options->flags);
		log_putln(0);
	}
}

void op_freeOpts(opts* options) {
	if (options) {
		free(options->options);
		free_string(options->flags);
		free(options);
	}	
}
