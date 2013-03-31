#include "../../include/stringptrlist.h"
#include "../../include/fileparser.h"

int stringptrlist_fromfile(stringptrlist* l, const char* fn) {
	fileparser f;
	stringptr line;
	int success = 0;
	if(fileparser_open(&f, fn)) goto err;
	while(!fileparser_readline(&f) && !fileparser_getline(&f, &line) && line.size) {
		if(!stringptrlist_add_strdup(l, &line)) goto ferr;
	}
	success = 1;
	ferr:
	fileparser_close(&f);
	err:
	return success;
}
