#ifndef TGILIST_H
#define TGILIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tglist.h"

/* this data structure is nothing more than a thin wrapper around tglist.
   it's a list providing quick means of insertion: by using a separate
   index table, the necessary re-shuffling happens only on integer values
   of user-chosen type, whereas the value list is only ever appended to.

   the usage of this list only makes sense if the value type is larger
   than the index type (e.g. with a struct).

   as an optimization, deleted items never vanish from the values list,
   they're simply no longer indexed.

   the index type could be chosen as e.g. short, if it's known that no
   more than 64K values will ever be added/deleted in total.
   this means it could be a security risk if it's not under your control.
   using size_t as the index fixes that, but is naturally slower.
*/

#define tgilist_impl(NAME, TYPE, IDX_TYPE) \
	struct NAME { \
		tglist_impl(, IDX_TYPE) index; \
		tglist_impl(, TYPE) items; \
	}

#define tgilist_decl(ID, TYPE, IDX_TYPE) tgilist_impl(tgilist_ ## ID, TYPE, IDX_TYPE)
#define tgilist(TYPE, IDX_TYPE) tgilist_impl(, TYPE, IDX_TYPE)
#define tgilist_proto tgilist_impl(, void*, size_t)

#define tgilist_getsize(X) tglist_getsize(&(X)->index)
#define tgilist_empty(X) (tgilist_getsize(X) == 0)

#define tgilist_new() calloc(1, sizeof(tgilist_proto))
#define tgilist_free(X) do { \
		tglist_free_items(&(X)->index); \
		tglist_free_items(&(X)->items); \
		free(X); } while(0)

#define tgilist_init(X) memset(X, 0, sizeof(*(X)))
#define tgilist_free_items(X) do {\
		tglist_free_items(&(X)->index); \
		tglist_free_items(&(X)->items); \
		} while(0)

#define tgilist_free_values(X) tglist_free_values(&(X)->items)

#define tgilist_get(X, POS) ((X)->items.items[(X)->index.items[POS]])

#define tgilist_set(X, ITEM, POS) ((tgilist_get(X, POS) = ITEM), 1)

#define tgilist_add(X, ITEM) ( \
	!tglist_add( &(X)->index, tglist_getsize(&(X)->items)) ? \
	0 : ( \
		(tglist_add( &(X)->items, ITEM)) ? 1 : \
			(tglist_delete(&(X)->index, tglist_getsize(&(X)->index)-1),  0) \
			) \
	)

/* only index items are only ever deleted before freeing the list */
#define tgilist_delete(X, POS) \
	tglist_delete(&(X)->index, POS)

#define tgilist_insert(X, ITEM, POS) ( \
	!(tglist_add(&(X)->items, ITEM)) ? \
	0 : ( \
		tglist_insert(&(X)->index, tglist_getsize(&(X)->items)-1, POS) ? 1 : \
		( --((X)->items.count), 0) \
		) \
	)

#define tgilist_itemsize(X) tglist_itemsize(&(X)->items)

#endif

