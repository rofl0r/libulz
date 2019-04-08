/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */


#ifndef _INIPARSER_H_
#define _INIPARSER_H_

#include <unistd.h>
#include "stringptr.h"
#include "stringptrlist.h"

typedef struct {
	size_t startline;
	size_t linecount;
} ini_section;

ini_section iniparser_file_as_section(stringptrlist* inifile);
ini_section iniparser_get_section(stringptrlist* inifile, const stringptr* sectionname);
ini_section iniparser_get_next_section(stringptrlist* inifile, const stringptr* sectionname, ini_section* prev);
void iniparser_getvalue(stringptrlist* inifile, ini_section* section, const stringptr* key, stringptr* result);
/* returns lineindex on success, -1 otherwise */
ssize_t iniparser_getvalue_at(stringptrlist* inifile, ini_section* section, const stringptr* key, stringptr* result, size_t startline);
size_t iniparser_count_sections(stringptrlist* inifile);

#pragma RcB2 DEP "../src/iniparser/*.c"

#endif

