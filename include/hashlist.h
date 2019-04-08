#ifndef HASHLIST_H
#define HASHLIST_H


#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#include "sblist.h"

typedef struct {
	sblist** buckets;
	uint32_t bucketcount;
	uint32_t datasize;
} hashlist;

typedef struct {
	ssize_t pos;
} hashlist_bucket_iterator;

typedef struct {
	ssize_t pos;
	hashlist_bucket_iterator bucket_iter;
} hashlist_iterator;

hashlist* hashlist_new(uint32_t bucketcount, uint32_t datasize);
/* returns -1 on error, 0 on success */
int hashlist_add(hashlist* h, uint32_t hash, void* value);
sblist* hashlist_get(hashlist* h, uint32_t hash);
void hashlist_free(hashlist* h);

void hashlist_bucket_iterator_init(hashlist_bucket_iterator* iter);
sblist* hashlist_bucket_next(hashlist* h, hashlist_bucket_iterator* iter);
void hashlist_iterator_init(hashlist_iterator* iter);
void* hashlist_next(hashlist* h, hashlist_iterator* iter);

#pragma RcB2 DEP "../src/hashlist/*.c"

#endif
