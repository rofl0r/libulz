#include "../include/iniparser.h"
#include "../include/stringptr.h"
#include "../include/strlib.h"
#include "../include/stringptrlist.h"

int main(int argc, char** argv) {
	stringptr* fc = stringptr_fromfile("test.ini");
	stringptrlist* lines = stringptr_linesplit(fc);
	ini_section sec = {0, 0};
	stringptr x;
	
	while(sec = iniparser_get_next_section(lines, SPL("section"), &sec), 
		sec.linecount) {
		iniparser_getvalue(lines, &sec, SPL("value"), &x);
		if(x.size)
			ulz_printf("value of section: %s\n", x.ptr);
	}
	stringptrlist_free(lines);
	stringptr_free(fc);
}
