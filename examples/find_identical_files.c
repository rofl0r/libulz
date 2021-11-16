#include "../include/filelist.h"
#include "../include/filelib.h"
#include "../include/logger.h"
#include <string.h>
#include <stdio.h>

static int stringptr_endswithchar(stringptr* s, int ch) {
	if(stringptr_getsize(s) == 0) return 0;
	return stringptr_get(s)[stringptr_getsize(s)-1] == ch;
}

static int compare_files(stringptr* fn1, stringptr* fn2) {
	FILE *f1 = fopen(stringptr_get(fn1), "r");
	FILE *f2 = fopen(stringptr_get(fn2), "r");
	int ret = 1;
	size_t n;
	while(1) {
		unsigned char buf1[16*1024];
		unsigned char buf2[16*1024];
		n = fread(buf1, 1, sizeof buf1, f1);
		if(n == 0) { ret = 0; break; }
		if(n != fread(buf2, 1, sizeof buf2, f2)) break;
		if(memcmp(buf1, buf2, n)) break;
	}
	fclose(f1);
	fclose(f2);
	return ret;
}

static int mode = 0; /* 0: identical, 1: not identical */

static int process_path(stringptr* dir1, stringptr* dir2) {
	filelist fl1, fl2;
	int ret;
	ret = filelist_search(&fl1, dir1, SPL("*"), FLF_EXCLUDE_PATH|FLF_INCLUDE_HIDDEN);
	if(ret) return ret;
	ret = filelist_search(&fl2, dir2, SPL("*"), FLF_EXCLUDE_PATH|FLF_INCLUDE_HIDDEN);
	if(ret) return ret;
	size_t i, j;
	for(i=0; i < stringptrlist_getsize(fl1.files); ++i) {
		stringptr *f1 = stringptrlist_get(fl1.files, i);
		if(!stringptrlist_contains(fl2.files, f1)) continue;
		stringptr* full1 = stringptr_concat(dir1, stringptr_endswithchar(dir1, '/') ? SPL("") : SPL("/"),  f1, NULL);
		stringptr* full2 = stringptr_concat(dir2, stringptr_endswithchar(dir1, '/') ? SPL("") : SPL("/"),  f1, NULL);
		if(stringptr_endswithchar(f1, '/')) {
			ret = process_path(full1, full2);
			if(ret) return ret;
			goto next;
		}
		if(getfilesize(stringptr_get(full1)) != getfilesize(stringptr_get(full2)))
			if(mode) goto print;
			else goto next;
		if(compare_files(full1, full2) == mode) {
		print:
			log_puts(1, full1);
			log_putln(1);
		}
	next:
		stringptr_free(full1);
		stringptr_free(full2);
	}
	filelist_free(&fl1);
	filelist_free(&fl2);
}

static int usage() {
	log_puts(2, SPL(
		"usage: PROG MODE DIR1 DIR2\n"
		"MODE: i - identical or n - not identical\n"
		"finds all (not) identical files in DIR1/2 and its subdirs.\n"
		"prints filenames of DIR1 that match MODE.\n"
		"files that exist only in DIR1 or DIR2 are excluded.\n"
	));
	return 1;
}

int main(int argc, char** argv) {
	if(argc != 4) return usage();
	mode = argv[1][0] == 'i' ? 0 : 1;
	SPDECLAREC(dir1, argv[2]);
	SPDECLAREC(dir2, argv[3]);
	int ret = process_path(dir1, dir2);
	return ret;
}
