#include "../include/filelist.h"
#include "../include/strlib.h"
#include "../include/logger.h"

typedef struct {
	stringptr srcdir_b;
	stringptr* srcdir;
} progstate_s;

static progstate_s progstate;

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

static void doDir(stringptr* subd) {
	filelist f;
	stringptr *combined_src = stringptr_concat(progstate.srcdir, subd, NULL);
	
	if(!filelist_search(&f, combined_src, SPL("*"), FLF_EXCLUDE_PATH | FLF_INCLUDE_HIDDEN)) {
		stringptr* file;
		stringptr* file_combined_src;
		log_puts(1, combined_src); log_putln(1); // for same output as find
		
		sblist_iter(f.files, file) {
			file_combined_src = stringptr_concat(combined_src, file, NULL);
			if(isdir(file)) {
				stringptr *path_combined = stringptr_concat(subd, file, NULL);
				doDir(path_combined);
				stringptr_free(path_combined);
			} else {
				log_puts(1, file_combined_src);  log_putln(1);
			}
			stringptr_free(file_combined_src);
		}
		filelist_free(&f);
	}
	
	stringptr_free(combined_src);
}

int main (int argc, char** argv) {
	if(argc != 2) return syntax();
	int startarg = 1;
	
	progstate.srcdir = stringptr_fromchar(argv[startarg], &progstate.srcdir_b);
	
	doDir(isdir(progstate.srcdir) ? SPL("") : SPL("/"));
	
	return 0;
}
