#ifndef HT_H
#define HT_H

#include <stdlib.h>
#include <stdint.h>

/*
Intro
-----

A generic hashtable for fixed-size numeric keys.

It's a variant of https://github.com/rofl0r/htab (the string-key version).

It provides generic types via macros, and very efficient storage:
the overhead per hashtable slot is only sizeof(htkey) + 2 bits.
A reversible hash function is used, which gives low-collision behavior,
and since it can be reversed the key doesn't need to be stored as well.
Performance is pretty good too, and can compete with the fastest ones,
if not beat them.

See the implementation part further down for copyright/license info.

Usage
-----

You can `#define` the key type `htkey` to any of:

* unsigned int
* unsigned long
* unsigned long long
* uint32_t
* uint64_t

The default uses `unsigned long`, which is adequate for most uses.
You should probably prefer an unsigned type over a signed one.

Signed key types are untested, but may work too.

The value type `htval` can be `#define`d to basically anything[*], e.g.
a struct, which will be stored inline in the hashtable.

If you want to manage the storage for structs yourself, use a pointer
type instead, or maybe even some integer type as index into your storage.

[*] except strings, since they have varying length. You can store
char pointers though, but then you have to manage their memory manually.

The default for `htval` is the union below, which can be used either
as a generic pointer or as a register-sized integer, using the two
accessor macros `HTV_N` (number access) or `HTV_P` (pointer access).

[WARNING]
Any pointer to values inside the hashtable becomes invalid the moment
you insert something new. Delete is fine though.

to export or not
----------------

This header-only library can be used in two ways:

* globally, with exported functions:
  all users share the same header, so you can only have one set of
  key/value types in your program to prevent name collisions.
  One of your translation units needs to `#define HTIMPL` before
  including this header, so the implementation is available for other
  consumers.

* locally, in several places with different key/value types each:
  in each translation unit that needs the hashtable, `#define HTEXPORT static`
  and `#define HTIMPL`. You'll get static versions of the code in each
  TU, so use that.
  If you need more than one hashtable type inside the same translation
  unit, you'll have to write a wrapper for each additional table version
  and include a header exposing the wrapper functions.

*/

#ifndef htkey
#define htkey unsigned long
#endif

#ifndef htval
#define htval ht_value
typedef union ht_value {
	void *p;
	size_t n;
} ht_value;

#define HTV_N(N) (ht_value){.n = (N)}
#define HTV_P(P) (ht_value){.p = (P)}
#endif

#ifndef HTEXPORT
#define HTEXPORT
#endif

/**
 * @brief Create a new hashtable.
 *
 * @param nel  initial number of elements requested.
 * @return pointer to the created hashtable, or NULL on allocation failure.
 */
HTEXPORT struct ht *ht_create(size_t nel);

/**
 * @brief Destroy a hashtable.
 *
 * @param ht  hashtable to destroy.
 */
HTEXPORT void ht_destroy(struct ht *ht);

/**
 * @brief Return the number of live items in the table.
 *
 * @param ht  hashtable to query.
 * @return number of live items.
 */
HTEXPORT size_t ht_count(const struct ht *ht);

/**
 * @brief Look up a key in the table.
 *
 * @param ht  hashtable to search.
 * @param key  key to look up.
 * @param out  receives a pointer to the stored value slot on success.
 * @return 1 if found, 0 otherwise.
 */
HTEXPORT int ht_find(struct ht *ht, htkey key, htval **out);

/**
 * @brief Insert a new key/value pair.
 *
 * @param ht  hashtable to update.
 * @param key  key to insert.
 * @param value  value to store.
 * @return 1 on success, 0 if no memory is available or the key already exists.
 */
HTEXPORT int ht_insert(struct ht *ht, htkey key, htval value);

/**
 * @brief Delete a key and associated value from the table.
 *
 * @param ht  hashtable to update.
 * @param key  key to delete.
 * @return 1 on success, 0 if the key was not found.
 */
HTEXPORT int ht_delete(struct ht *ht, htkey key);

/**
 * @brief Iterate over the table entries.
 *
 * @param ht  hashtable to iterate.
 * @param iterator  iteration state; pass 0 to start.
 * @param key  receives the key of the current entry.
 * @param v  receives a pointer to the stored value slot.
 * @return next iterator value, or 0 when the iteration is finished.
 */
HTEXPORT size_t ht_next(struct ht *ht, size_t iterator, htkey *key, htval **v);

#endif /* header part end */

/*
musl license, hsearch.c originally written by Szabolcs Nagy

Copyright © 2005-2020 Rich Felker, Szabolcs Nagy : musl hsearch.c
Copyright © 2020-2026 rofl0r : anything else

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef HTIMPL

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

/*
Open addressing hash table with 2^n table size and quadratic probing.
Uses reversible hash functions so only the hash needs to be stored,
and the key can be computed.
a separate 2-bit bitmap keeps the bookkeeping state. */

struct elem {
	htkey hash;
	htval data;
};

struct ht {
	struct elem *elems;
	uint8_t *state;   /* allocated at the tail of elems */
	size_t mask;      /* slots-1 */
	size_t used;      /* live occupied count */
	size_t tombs;     /* tombstone count */
};

#define MINSIZE 8
#define MAXSIZE ((size_t)-1/2 + 1)

/* 2-bit states */
#define S_EMPTY     0u
#define S_OCCUPIED  1u
#define S_TOMBSTONE 2u

/* reversible hash functions */
static uint64_t hash64(uint64_t x) {
	x = (x ^ (x >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
	x = (x ^ (x >> 27)) * UINT64_C(0x94d049bb133111eb);
	x = x ^ (x >> 31);
	return x;
}

static uint64_t unhash64(uint64_t x) {
	x = (x ^ (x >> 31) ^ (x >> 62)) * UINT64_C(0x319642b2d24d8ec3);
	x = (x ^ (x >> 27) ^ (x >> 54)) * UINT64_C(0x96de1b173f119089);
	x = x ^ (x >> 30) ^ (x >> 60);
	return x;
}

static uint32_t hash32(uint32_t x) {
	x ^= x >> 16;
	x *= 0x7feb352du;
	x ^= x >> 15;
	x *= 0x846ca68bu;
	x ^= x >> 16;
	return x;
}

static uint32_t unhash32(uint32_t x) {
	x ^= x >> 16;
	x *= 0x43021123u;
	x ^= x >> 15 ^ x >> 30;
	x *= 0x1d69e2a5u;
	x ^= x >> 16;
	return x;
}

static htkey keyhash(htkey k)
{
	if ((htkey)-1 == 0xffffffff)
		return (htkey)hash32(k);
	return (htkey)hash64(k);
}

static htkey keyunhash(htkey h)
{
	if ((htkey)-1 == 0xffffffff)
		return (htkey)unhash32(h);
	return (htkey)unhash64(h);
}

/* helpers for 2-bit slot state */
static unsigned get_state(const uint8_t *st, size_t idx)
{
	size_t byte = idx >> 2;
	unsigned shift = (idx & 3u) << 1;
	return (st[byte] >> shift) & 3u;
}

static void set_state(uint8_t *st, size_t idx, unsigned v)
{
	size_t byte = idx >> 2;
	unsigned shift = (idx & 3u) << 1;
	st[byte] = (uint8_t)((st[byte] & ~(3u << shift)) | ((v & 3u) << shift));
}

/* core table functions */

/* allocate one zeroed block containing elems[] followed by state bitmap */
static int alloc_table(struct ht *ht, size_t slots)
{
	size_t stbytes = (slots + 3) >> 2; /* 2-bit states => ceil(slots/4) bytes */
	size_t stateslots = (stbytes + sizeof(struct elem) - 1) / sizeof(struct elem);
	if (!stateslots) stateslots = 1;
	struct elem *mem = calloc(sizeof(struct elem), slots + stateslots);
	if (!mem) return 0;
	ht->elems = mem;
	ht->state = (void*)(mem + slots);
	return 1;
}

static int resize(struct ht *ht, size_t nel)
{
	size_t newsize, i, j;
	struct elem *oldtab = ht->elems;
	uint8_t *oldst = ht->state;
	size_t oldsize = oldtab ? (ht->mask + 1) : 0;

	if (nel > MAXSIZE) nel = MAXSIZE;
	for (newsize = MINSIZE; newsize < nel; newsize *= 2);
	/* if newsize == oldsize, a rebuild to remove tombstones is needed */

	struct ht tmp = *ht;
	tmp.elems = NULL;
	tmp.state = NULL;

	if (!alloc_table(&tmp, newsize))
		return 0;

	tmp.mask = newsize - 1;
	tmp.used = ht->used;
	tmp.tombs = 0;

	if (!oldtab) goto commit;

	/* re-insert occupied elements */
	for (size_t k = 0; k < oldsize; k++) {
		if (get_state(oldst, k) != S_OCCUPIED)
			continue;

		htkey h = oldtab[k].hash;

		for (i = (size_t)h, j = 1; ; i += j++) {
			size_t pos = i & tmp.mask;
			if (get_state(tmp.state, pos) == S_EMPTY) {
				tmp.elems[pos] = oldtab[k];
				set_state(tmp.state, pos, S_OCCUPIED);
				break;
			}
		}
	}
	free(oldtab);
commit:
	*ht = tmp;
	return 1;
}

/*
lookup:
- returns slot index
- sets *state to:
    S_OCCUPIED  : found existing entry
    S_EMPTY     : miss; returned slot was EMPTY
    S_TOMBSTONE : miss; returned slot was a TOMBSTONE to be reused
*/
static size_t lookup(struct ht *ht, htkey h, unsigned *state)
{
	size_t i, j;
	size_t tomb = (size_t)-1;

	for (i = (size_t)h, j = 1; ; i += j++) {
		size_t pos = i & ht->mask;
		*state = get_state(ht->state, pos);
		switch(*state) {
		case S_EMPTY:
			if (tomb != (size_t)-1) {
				*state = S_TOMBSTONE;
				return tomb;
			}
			return pos;
		case S_TOMBSTONE:
			if (tomb == (size_t)-1) tomb = pos;
			continue;
		case S_OCCUPIED:
			if (ht->elems[pos].hash == h)
				return pos;
			break;
		}
	}
}

/* returns 0 if no resize/rebuild is needed, else the target nel to pass
   to resize() */
static size_t need_resize(const struct ht *ht)
{
	size_t nonempty = ht->used + ht->tombs;

	/* grow when live load exceeds 75% */
	if (ht->used > ht->mask - ht->mask/4) {
		size_t nel = 2 * ht->used;
		if (!nel) nel = MINSIZE;
		return nel;
	}

	/* rebuild when nonempty exceeds 75% */
	if (nonempty > ht->mask - ht->mask/4) {
		return ht->used ? ht->used : MINSIZE;
	}

	return 0;
}

/*public API */

HTEXPORT struct ht *ht_create(size_t nel)
{
	struct ht *r = calloc(1, sizeof *r);
	if (!r) return NULL;
	if (!resize(r, nel)) {
		free(r);
		return NULL;
	}
	return r;
}

HTEXPORT void ht_destroy(struct ht *ht)
{
	if (!ht) return;
	free(ht->elems);
	free(ht);
}

HTEXPORT size_t ht_count(const struct ht *ht)
{
	return ht ? ht->used : 0;
}

HTEXPORT int ht_find(struct ht *ht, htkey key, htval **out)
{
	htkey h = keyhash(key);
	unsigned state;
	size_t pos = lookup(ht, h, &state);
	if (state != S_OCCUPIED) return 0;
	if (out) *out = &ht->elems[pos].data;
	return 1;
}

HTEXPORT int ht_delete(struct ht *ht, htkey key)
{
	htkey h = keyhash(key);
	unsigned state;
	size_t pos = lookup(ht, h, &state);
	if (state != S_OCCUPIED) return 0;

	set_state(ht->state, pos, S_TOMBSTONE);
	ht->used--;
	ht->tombs++;
	return 1;
}

HTEXPORT int ht_insert(struct ht *ht, htkey key, htval value)
{
	size_t nel = need_resize(ht);
	if (nel && !resize(ht, nel))
		return 0;

	htkey h = keyhash(key);
	unsigned state;
	size_t pos = lookup(ht, h, &state);

	if (state == S_OCCUPIED) return 0; /* no overwrite */

	ht->elems[pos].hash = h;
	ht->elems[pos].data = value;
	set_state(ht->state, pos, S_OCCUPIED);

	ht->used++;
	if (state == S_TOMBSTONE)
		ht->tombs--;

	return 1;
}

HTEXPORT size_t ht_next(struct ht *ht, size_t iterator, htkey *key, htval **v)
{
	size_t slots = ht->mask + 1;
	for (size_t i = iterator; i < slots; i++) {
		if (get_state(ht->state, i) == S_OCCUPIED) {
			if (key) *key = keyunhash(ht->elems[i].hash);
			if (v) *v = &ht->elems[i].data;
			return i + 1;
		}
	}
	return 0;
}

#endif /* end of HTIMPL part */
