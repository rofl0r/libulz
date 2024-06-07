#include "../../include/sblist.h"
#include <stdlib.h>

sblist *sblist_new_from_data(size_t itemsize, void *data, size_t datalen) {
	sblist* ret = (sblist*) malloc(sizeof(sblist));
	if(!ret) return 0;
	ret->itemsize = itemsize;
	ret->blockitems = 0;
	ret->capa = ret->count = datalen / itemsize;
	ret->items = data;
	return ret;
}
