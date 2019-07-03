#include "../../include/sblist.h"
#include <stdlib.h>

void sblist_free_values(sblist *l) {
	size_t i;
	for(i=0; i<sblist_getsize(l); i++)
		free(sblist_get(l, i));
}
