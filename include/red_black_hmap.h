#ifndef RED_BLACK_HMAP_H_
#define RED_BLACK_HMAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* RedBlackNodeFactory */
#include "red_black_hmap_count.h"   /* RedBlackHMapCount */
#include "red_black_iterator.h"	    /* RedBlackIterator */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHBucket {
	struct RedBlackNode *restrict root;
	struct RedBlackNodeFactory node_factory;
};

struct _RedBlackHMap {
	struct RedBlackHBucket *restrict buckets;
	struct RedBlackHMapCount count;
};
typedef struct _RedBlackHMap RedBlackHMap;

struct _RedBlackHMapIterator {
	struct RedBlackIterator bucket_iter;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;
};
typedef struct _RedBlackHMapIterator RedBlackHMapIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hmap_init(RedBlackHMap *const restrict map);

void
red_black_hmap_destroy(RedBlackHMap *const restrict map);

int
red_black_hmap_insert(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_update(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict old_ptr);

int
red_black_hmap_delete(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_remove(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict key_ptr);

bool
red_black_hmap_find(RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length);

bool
red_black_hmap_fetch(RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict key_ptr);

unsigned int
red_black_hmap_count(RedBlackHMap *const restrict map);


bool
red_black_hmap_verify(RedBlackHMap *const restrict map);

void
red_black_hmap_iterator_init(RedBlackHMapIterator *const restrict iter,
			     RedBlackHMap *const restrict map);

bool
red_black_hmap_iterator_next(RedBlackHMapIterator *const restrict iter,
			     void **const restrict key_ptr,
			     size_t *const restrict length_ptr);
#endif /* ifndef RED_BLACK_HMAP_H_ */
