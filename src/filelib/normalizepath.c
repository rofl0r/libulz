#include "../../include/filelib.h"
#include "../../include/stringptr.h"
#include <limits.h>

/* removes multiple slashes, /./ style stuff and resolves ../ kind of stuff on _absolute_ paths.
 * will always strip trailing slashes.
 * will fail with relative paths. */
int normalizepath(stringptr* path) {
	char buf[PATH_MAX];
	size_t i;
	stringptr out;
	char* tmp;
	out.ptr = buf;
	out.size = 0;
	if(!path->size || path->ptr[0] != '/') goto fail;
	for(i = 0; i < path->size; i++) {
		switch(path->ptr[i]) {
			case '/':
				while(path->ptr[i+1] == '/') i++;
				if(!out.size || out.ptr[out.size -1] != '/') goto copy;
				break;
			case '.':
				if(!i || !out.size) goto fail;
				else if(path->ptr[i-1] == '/' && out.ptr[out.size -1] == '/') {
					out.size--;
					switch(path->ptr[i+1]) {
						case '.':
							if((!path->ptr[i+2] || path->ptr[i+2] == '/')) {
								if((tmp = stringptr_rchr(&out, '/'))) {
									out.size = tmp - out.ptr;
									i += 2;
									goto restore;
								} else {
									fail:
									return 0;
								}
							}
							goto def;
						case '/':
							i++;
							restore:
							out.size++;
							break;
						case 0:
							goto restore;
						default:
							def:
							out.size++;
							goto copy;
							break;
					}
				} else goto copy;
				break;
			default:
				copy:
				out.ptr[out.size++] = path->ptr[i];
				break;
		}
	}
	if(out.size > 1 && out.ptr[out.size -1] == '/')
		out.size--; // remove trailing slash
	out.ptr[out.size] = 0;
	memcpy(path->ptr, out.ptr, out.size + 1);
	path->size = out.size;
	return 1;
}
