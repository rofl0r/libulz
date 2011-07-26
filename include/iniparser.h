/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */


#ifndef _INIPARSER_H_
#define _INIPARSER_H_

#include "stringptr.h"
#include "stringptrlist.h"

typedef struct {
	size_t startline;
	size_t linecount;
} ini_section;

ini_section iniparser_get_section(stringptrlist* inifile, stringptr* sectionname);
void iniparser_getvalue(stringptrlist* inifile, ini_section* section, stringptr* key, stringptr* result);

#endif

//RcB: DEP "../src/iniparser/*.c"