#include "../../include/filelib.h"
#include "../../include/strlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

char* ulz_mkdtemp(char* templ) {
	size_t i, l = strlen(templ);
	if(l < 6) {
		errno = EINVAL;
		return NULL;
	}
	loop:
	for(i = l - 6; i < l; i++) templ[i] = conv_cypher[rand() % conv_cypher_len];
	if(mkdir(templ, S_IRWXU) == -1) {
		if(errno == EEXIST) goto loop;
		return NULL;
	}
	return templ;
}
