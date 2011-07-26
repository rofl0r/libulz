#include "../../include/optparser.h"
#include "../../include/strlib.h"

opts* op_parse(int argc, char** argv) {
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
	result->flags = stringptr_new(fc);
	result->options = stringptrlist_new(oc * 2);
	result->options->size = oc*2;
	fp = result->flags->ptr;
	// 2nd pass: write struct
	for(i=1;i<argc;i++) {
		op = argv[i];
		if(*op == '-') {
			op++;
			if((np = strchr(op, '='))) {
				stringptrlist_set(result->options,lcur++,op, np-op);
				*np = '\0';
				np++;
				stringptrlist_set(result->options,lcur++,np, strlen(np));
			} else {
				l = strlen(op);
				memcpy(fp, op, l);
				fp += l;
			}
		}
	}
	return result;
}
