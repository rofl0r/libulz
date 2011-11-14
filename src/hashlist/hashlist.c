#include "../../include/hashlist.h"

/* returns a new hashlist. only one possible datasize allowed, i.e. all stored values must have the same length */
hashlist* hashlist_new(uint32_t bucketcount, uint32_t datasize) {
	hashlist* res = calloc(1, sizeof(hashlist));
	if(!res) return NULL;
	res->buckets = calloc(bucketcount, sizeof(sblist*));
	if(!res->buckets) {
		free(res);
		return NULL;
	}
	res->bucketcount = bucketcount;
	res->datasize = datasize;
	return res;
}

/* returns -1 on error, 0 on success */
int hashlist_add(hashlist* h, uint32_t hash, void* value) {
	size_t dstbkt = hash % h->bucketcount;
	if(h->buckets[dstbkt] == NULL) {
		h->buckets[dstbkt] = sblist_new(h->datasize, 8);
		if(!h->buckets[dstbkt]) return -1;
	}
	return !sblist_add(h->buckets[dstbkt], value);
}

/* return a sblist containing all values matching the hash, or NULL if not found */
sblist* hashlist_get(hashlist* h, uint32_t hash) {
	return h->buckets[hash % h->bucketcount];
}

void hashlist_free(hashlist* h) {
	size_t i;
	for(i = 0; i < h->bucketcount; i++) {
		if(h->buckets[i]) sblist_free(h->buckets[i]);
	}
}


