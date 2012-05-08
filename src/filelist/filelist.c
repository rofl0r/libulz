#include <string.h>
#include "../../include/filelist.h"

/*
int errfunc(const char *epath, int eerrno) {
	return 0;
}
*/
// returns 0 on success, -1 on alloc or pointer failure, otherwise glob errors as defined in man 3p glob
// dir can be NULL or empty, this chooses the local directory then.
int filelist_search(filelist* l, stringptr* dir, stringptr* mask, int flags) {
	char buf[256];
	char *o;
	int ret;
	size_t i;
	stringptr temp, *s = &temp;
	int isdir;
	int defaultflags = (flags & FLF_INCLUDE_HIDDEN) ? GLOB_PERIOD : 0;
	
	if(!dir || !dir->size) dir = SPL("./");
	
	if(!l || !mask || !mask->size || mask->size + dir->size + (dir->ptr[dir->size -1] != '/') + 1U > sizeof(buf)) return -1;
	memcpy(buf, dir->ptr, dir->size);
	o = buf + dir->size;
	if(dir->ptr[dir->size -1] != '/') {
		*o = '/';
		o++;
	}
	memcpy(o, mask->ptr, mask->size);
	o += mask->size;
	*o = 0;
	if((ret = glob(buf, defaultflags | GLOB_MARK | GLOB_NOESCAPE, NULL, &l->pglob))) return ret;
	if(!(l->files = stringptrlist_new(l->pglob.gl_pathc))) {
		err:
		globfree(&l->pglob);
		return -1;
	}
	if(!(flags & FLF_INCLUDE_HIDDEN)) {
		// the strings in the filelist are managed by glob, we dont free them ourselves.
		// so its save to add string literals.
		if(flags & FLF_INCLUDE_DOT) stringptrlist_add(l->files, "./", 2);
		if(flags & FLF_INCLUDE_DOT_DOT) stringptrlist_add(l->files, "../", 3);
	}
	for(i = 0; i < l->pglob.gl_pathc; i++) {
		s->ptr = l->pglob.gl_pathv[i];
		s->size = strlen(l->pglob.gl_pathv[i]);
		if(!s->size) continue;
		if(!(flags & FLF_EXCLUDE_PATH)) {
			// check for xxxx/./ and xxx/../
			if((flags & FLF_INCLUDE_HIDDEN) && s->size >= 3 && s->ptr[s->size -1] == '/' &&
				s->ptr[s->size -2] == '.') {
				if(
					((!(flags & FLF_INCLUDE_DOT)) && s->ptr[s->size-3] == '/') ||
					((!(flags & FLF_INCLUDE_DOT_DOT)) && s->size >= 4 && s->ptr[s->size-3] == '.' && s->ptr[s->size-4] == '/')
				) continue;
			}
		} else {
			if((isdir = s->ptr[s->size - 1] == '/')) {
				// remove slash to have rchr work right
				s->ptr[s->size - 1] = 0;
				s->size--;
			}
			if((o = stringptr_rchr(s, '/'))) {
				o++;
				if(isdir) {
					// restore slash
					s->size++;
					s->ptr[s->size - 1] = '/';
				}
				s->size = (s->ptr + s->size) - o;
				s->ptr = o;
			} else {
				// this should never happen, since we exchange an empty dir with "./"
				goto err;
			}
			if(flags & FLF_INCLUDE_HIDDEN && s->ptr[0] == '.') {
				if(
					((!(flags & FLF_INCLUDE_DOT)) && s->size == 2 && s->ptr[1] == '/') ||
					((!(flags & FLF_INCLUDE_DOT_DOT)) && s->size == 3 && s->ptr[1] == '.' && s->ptr[2] == '/')
				) continue;
			}
		}
		if(!stringptrlist_add(l->files, s->ptr, s->size)) {
			free(l->files);
			goto err;
		}
	}
	return 0;
}

void filelist_free(filelist* l) {
	if(l) {
		if(l->files) stringptrlist_free(l->files);
		globfree(&l->pglob);
	}
}

