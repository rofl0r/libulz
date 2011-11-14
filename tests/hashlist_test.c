#include "../include/hashlist.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
	uint32_t hash;
	uint32_t value;
} mystruct;

int main() {
	
	hashlist* h; 
	hashlist_iterator i;
	mystruct m, *p;
	h = hashlist_new(16, sizeof(mystruct));
	
	m.hash = 1;
	m.value = 1;
	hashlist_add(h, m.hash, &m);
	m.hash = 1;
	m.value = 2;
	hashlist_add(h, m.hash, &m);
	
	hashlist_iterator_init(&i);
	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 1 && p->value == 1);

	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 1 && p->value == 2);

	p = hashlist_next(h, &i);
	assert(!p);
	
	m.hash = 15;
	m.value = 1;
	hashlist_add(h, m.hash, &m);
	m.value = 2;
	hashlist_add(h, m.hash, &m);
	m.value = 3;
	hashlist_add(h, m.hash, &m);

	hashlist_iterator_init(&i);
	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 1 && p->value == 1);

	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 1 && p->value == 2);
	
	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 15 && p->value == 1);

	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 15 && p->value == 2);

	p = hashlist_next(h, &i);
	assert(p);
	assert(p->hash == 15 && p->value == 3);
	

	p = hashlist_next(h, &i);
	assert(!p);
	
	
	return 0;
}