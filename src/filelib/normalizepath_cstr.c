#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <string.h>

/* removes multiple slashes, /./ style stuff and resolves ../ kind of stuff on _absolute_ paths.
 * will always strip trailing slashes.
 * will fail with relative paths. does not resolve symlinks.
 * out must point to a buffer of sufficient size, which equals (worst case) strlen(path)+1.
 * returns 0 on failure, on success out.
*/
char* normalizepath_cstr(const char* path, char* out) {
	size_t i, out_size = 0;
	char* tmp;
#if defined(__psp__) || defined(PSPLIKE_PATH)
	/* psp paths are a mix of DOS and unix, with a drive name + colon,
	   followed by a unix path, e.g. ms0:/PSP */
	for(i = 0; path[i]; ++i) {
		out[out_size++] = path[i];
		if(path[i] == '/') {
			--out_size;
			if(i > 0 && path[i-1] != ':') goto fail;
			break;
		}
	}
#else
	i = 0;
#endif
	if(path[i] != '/') goto fail;
	for(; path[i]; i++) {
		switch(path[i]) {
		case '/':
			while(path[i+1] == '/') i++;
			if(!out_size || out[out_size -1] != '/') goto copy;
			break;
		case '.':
			if(!i || !out_size) goto fail;
			else if(path[i-1] == '/' && out[out_size -1] == '/') {
				out_size--;
				switch(path[i+1]) {
					case '.':
						if((!path[i+2] || path[i+2] == '/')) {
							tmp = out + out_size;
							while(--tmp > out && *tmp != '/');
							if(tmp >= out && *tmp == '/') {
								out_size = tmp - out;
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
						out_size++;
						if(!path[i]) goto done;
						break;
					case 0:
						goto restore;
					default:
						def:
						out_size++;
						goto copy;
						break;
				}
			} else goto copy;
			break;
		default:
			copy:
			out[out_size++] = path[i];
			break;
		}
	}
	done:
	if(out_size > 1 && out[out_size -1] == '/')
		out_size--; // remove trailing slash
	out[out_size] = 0;
	return out;
}

#ifdef FUZZER_TEST
#include <sys/mman.h>
#include <stdio.h>
int main(int a, char**b) {
	char buf[64], wb[64];
	if(a == 3 && b[1][0] == 'X' && !b[1][1]) {
		if(normalizepath_cstr(b[2], buf))
			printf("%s -> %s\n" , b[2], buf);
		return 0;
	}
	FILE *f;
	if(a == 2 && (f = fopen(b[1], "r")) && fgets(buf, sizeof buf, f)) {
		size_t l = strlen(buf); if(l) buf[l-1] = 0; else return 1;
		char *page2 = mmap(0, 4096, PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		char* page = mmap(0, 4096, PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		if(!page) return 1;
		if(!page2) return 1;
		if(page2-page!=4096) return 1;
		char* ns = page+4096-l;
		memcpy(ns, buf, l);
		if (normalizepath_cstr(ns, wb))
			printf("%s -> %s\n" , ns, wb);
		fclose(f);
		return 0;
	}
	return 1;
}
#endif
