#include "../include/filelist.h"
#include "../include/strlib.h"
#include "../include/logger.h"

static int isdir(stringptr* file) {
	return file->size && file->ptr[file->size -1] == '/';
}

static int syntax() {
	log_puts(1, SPL(
	"prog dir\n"
	"lists all files in dir. the output should be equivalent to\n"
	"find dir -name '*'\n"
	"at least regarding number of returned entries.\n"
	));
	return 1;
}

static void doDir(stringptr* dir) {
	filelist f;
	
	if(!filelist_search(&f, dir, SPL("*"), FLF_INCLUDE_HIDDEN)) {
		stringptr* file;
		log_puts(1, dir); log_putln(1); // for same output as find
		
		sblist_iter(f.files, file) {
			if(isdir(file)) {
				doDir(file);
			} else {
				log_puts(1, file);  log_putln(1);
			}
		}
		filelist_free(&f);
	}
}

int main (int argc, char** argv) {
	if(argc != 2) return syntax();
	int startarg = 1;
	stringptr srcdir_b;
	stringptr* srcdir;
	stringptr* tmp = NULL;
	
	srcdir = stringptr_fromchar(argv[startarg], &srcdir_b);
	
	doDir(isdir(srcdir) ? srcdir : (tmp = stringptr_concat(srcdir , SPL("/"), NULL)));
	if(tmp) stringptr_free(tmp);
	
	return 0;
}
