#ifndef _INIPARSER_H_
#define _INIPARSER_H_

#include "stringptr.h"

typedef struct {
	size_t startline;
	size_t linecount;
} ini_section;

ini_section iniparser_get_section(stringptrlist* inifile, stringptr* sectionname);
void iniparser_getvalue(stringptrlist* inifile, ini_section* section, stringptr* key, stringptr* result);

#endif