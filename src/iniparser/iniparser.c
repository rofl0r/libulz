/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */

#include <string.h>
#include "../../include/iniparser.h"

ini_section iniparser_get_section_at(stringptrlist* inifile, stringptr* sectionname, size_t startline) {
	ini_section result = {0, 0};
	stringptr* line;
	size_t i;
	for(i = startline; i < stringptrlist_getsize(inifile); i++) {
		line = stringptrlist_get(inifile, i);
		if(!line) continue; // this should never happen...
		if(
			!result.startline &&
			line->size == (sectionname->size + 2) &&
			*line->ptr == '[' && *(line->ptr + 1 + sectionname->size) == ']' &&
			!memcmp(line->ptr + 1, sectionname->ptr, sectionname->size)
		)
			result.startline = i+1;
		else if (result.startline && line->size && *line->ptr == '[') {
			result.linecount = i - result.startline;
			goto done;
		}
	}
	if(result.startline && !result.linecount)
		result.linecount = stringptrlist_getsize(inifile) - result.startline;
	done:
	return result;
}

ini_section iniparser_get_section(stringptrlist* inifile, stringptr* sectionname) {
	return iniparser_get_section_at(inifile, sectionname, 0);
}

ini_section iniparser_get_next_section(stringptrlist* inifile, stringptr* sectionname, ini_section* prev) {
	return iniparser_get_section_at(inifile, sectionname, prev->startline + prev->linecount);
}

void iniparser_getvalue(stringptrlist* inifile, ini_section* section, stringptr* key, stringptr* result) {
	size_t i;
	size_t maxline = section->startline + section->linecount;
	stringptr* line;
	for(i = section->startline; i < maxline; i++) {
		line = stringptrlist_get(inifile, i);
		if(line && line->size > key->size && *(line->ptr + key->size) == '=' && !memcmp(line->ptr, key->ptr, key->size)) {
			result->size = line->size - (key->size + 1);
			result->ptr = line->ptr + key->size + 1;
			return;
		}
	}
	result->size = 0;
	result->ptr = NULL;
}

