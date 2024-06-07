#include <stdlib.h>
#include <string.h>
#include "../../include/tlist.h"

#ifndef UINT_MAX
#define UINT_MAX 0xffffffffU
#endif

static int mrand(unsigned *seed)
{
       return (*seed = (*seed+1) * 1103515245 + 12345 - 1)+1 & 0x7fffffff;
}

typedef struct item* pitem;
struct item {
	unsigned prior, cnt;
	pitem l, r;
};

static unsigned cnt (pitem it) {
	return it ? it->cnt : 0;
}

static void upd_cnt (pitem it) {
	if (it)
		it->cnt = cnt(it->l) + cnt(it->r) + 1;
}

static void merge (pitem *t, pitem l, pitem r) {
	if (!l || !r)
		*t = l ? l : r;
	else if (l->prior > r->prior)
		merge (&l->r, l->r, r),  *t = l;
	else
		merge (&r->l, l, r->l),  *t = r;
	upd_cnt (*t);
}

static void split (pitem t, pitem *l, pitem *r, unsigned key, unsigned add) {
	if (!t) {
		*l = *r = 0;
		return;
	}
	unsigned cur_key = add + cnt(t->l);
	if (key <= cur_key)
		split (t->l, l, &t->l, key, add),  *r = t;
	else
		split (t->r, &t->r, r, key, add + 1 + cnt(t->l)),  *l = t;
	upd_cnt (t);
}

static pitem getitem(pitem t, unsigned idx, unsigned add) {
	if (!t) return t;
	unsigned ls = cnt (t->l), cur_key = add + ls;
	if (cur_key == idx) return t;
	if (cur_key < idx)
		return getitem (t->r, idx, add + 1 + ls);
	else
		return getitem (t->l, idx, add);
}

static void insert(pitem *t, pitem n, unsigned idx) {
	pitem t1, t2;
	split (*t, &t1, &t2, idx, 0);
	merge (t, t1, n);
	merge (t, *t, t2);
}

static void remove(pitem *t, unsigned idx, unsigned add) {
	pitem n;
	if (!(*t)) return;
	unsigned cur_key = add + cnt ((*t)->l), new_add = cur_key + 1;
	unsigned rk, lk = rk = UINT_MAX;
	if ((*t)->l) lk = cnt ((*t)->l->l) + add;
	if ((*t)->r) rk = cnt ((*t)->r->l) + new_add;
	if (cur_key == idx) {
		merge (t, (*t)->l, (*t)->r);
	} else if (lk == idx) {
		merge (&n, (*t)->l->l, (*t)->l->r);
		(*t)->l = n;
		upd_cnt (*t);
	} else if (rk == idx) {
		merge (&n, (*t)->r->l, (*t)->r->r);
		(*t)->r = n;
		upd_cnt (*t);
	} else if (cur_key < idx) {
		remove (&(*t)->r, idx, new_add);
		upd_cnt (*t);
	} else {
		remove (&(*t)->l, idx, add);
		upd_cnt (*t);
	}
}

static pitem new_item(void* value, unsigned valsz, unsigned *seed) {
	pitem n = malloc(sizeof(struct item) + valsz);
	if(!n) return n;
	memcpy(n+1, value, valsz);
	n->prior = mrand(seed);
	n->cnt = 1;
	n->l = n->r = 0;
	return n;
}

struct tlist {
	unsigned seed;
	unsigned itemsize;
	pitem root;
};

struct tlist *tlist_new(unsigned itemsize) {
	struct tlist* new = malloc(sizeof (struct tlist));
	if(!new) return 0;
	new->seed = 385-1;
	new->itemsize = itemsize;
	new->root = 0;
	return new;
}

static void* data(pitem it) {
	return it+1;
}

size_t tlist_getsize(struct tlist* t) {
	return cnt(t->root);
}

void* tlist_get(struct tlist* t, size_t idx) {
	return data(getitem(t->root, idx, 0));
}

int tlist_insert(struct tlist* t, size_t idx, void *value) {
	if(idx > cnt (t->root)) return 0;
	pitem new = new_item(value, t->itemsize, &t->seed);
	if(!new) return 0;
	insert(&t->root, new, idx);
	return 1;
}

int tlist_append(struct tlist* t, void *value) {
	return tlist_insert(t, cnt(t->root), value);
}

static int tlist_delete_impl(struct tlist *t, size_t idx, int deep) {
	if(idx >= cnt (t->root)) return 0;
	pitem it = getitem(t->root, idx, 0);
	if(deep) free(data(it));
	remove(&t->root, idx, 0);
	free(it);
	return 1;
}

int tlist_delete(struct tlist *t, size_t idx) {
	return tlist_delete_impl(t, idx, 0);
}

int tlist_delete_deep(struct tlist *t, size_t idx) {
	return tlist_delete_impl(t, idx, 1);
}

static void tlist_free_items_impl(struct tlist *t, int deep) {
	while(cnt(t->root)) tlist_delete_impl(t, 0, deep);
}

void tlist_free_items(struct tlist *t) {
	tlist_free_items_impl(t, 0);
}

void tlist_free_items_deep(struct tlist *t) {
	tlist_free_items_impl(t, 1);
}

static void* tlist_free_impl(struct tlist *t, int deep) {
	tlist_free_items_impl(t, deep);
	free(t);
	return 0;
}

void *tlist_free(struct tlist *t) {
	return tlist_free_impl(t, 0);
}

void *tlist_free_deep(struct tlist *t) {
	return tlist_free_impl(t, 1);
}

#ifdef TLIST_TEST
extern int printf(const char *__restrict, ...);
float tlist_getbalance(struct tlist *t) {
	size_t n = tlist_getsize(t);
	int r, l;
	if (n == 0) return 1.0;
	l = cnt (t->root->l);
	r = cnt (t->root->r);
	printf("l %d, r %d, diff %d\n", l, r, abs(l-r));
	return 100.f - ((float)abs(l - r)/(n/100.f));
}
#endif
