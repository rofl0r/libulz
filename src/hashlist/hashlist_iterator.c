#include "../../include/hashlist.h"

void hashlist_bucket_iterator_init(hashlist_bucket_iterator* iter) {
	iter->pos = -1;
}

sblist* hashlist_bucket_next(hashlist* h, hashlist_bucket_iterator* iter) {
	iter->pos++;
	while((size_t) iter->pos < h->bucketcount && h->buckets[iter->pos] == NULL) iter->pos++;
	if((size_t) iter->pos < h->bucketcount) return h->buckets[iter->pos];
	return NULL;
}

void hashlist_iterator_init(hashlist_iterator* iter) {
	iter->bucket_iter.pos = -1;
	iter->pos = -1;
}

void* hashlist_next(hashlist* h, hashlist_iterator* iter) {
	if(iter->bucket_iter.pos >= (ssize_t) h->bucketcount) return NULL;
	iter->pos++;
	if(iter->bucket_iter.pos == -1 || (size_t) iter->pos >= sblist_getsize(h->buckets[iter->bucket_iter.pos])) {
		iter->pos = 0;
		if(!hashlist_bucket_next(h, &iter->bucket_iter)) return NULL;
	}
	return sblist_get(h->buckets[iter->bucket_iter.pos], iter->pos);
}

