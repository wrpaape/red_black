#ifndef RED_BLACK_LHMAP_H_
#define RED_BLACK_LHMAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_lock.h"         /* RedBlackLock */
#include "red_black_node_factory.h" /* RedBlackNodeFactory */
#include "red_black_hmap_count.h"   /* RedBlackHMapCount */
#include "red_black_iterator.h"	    /* RedBlackIterator */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackLHBucket {
	RedBlackLock lock;
	struct RedBlackNode *restrict root;
	struct RedBlackNodeFactory node_factory;
};

struct _RedBlackLHMap {
	RedBlackLock lock;
	struct RedBlackLHBucket *restrict buckets;
	struct RedBlackHMapCount count;
};
typedef struct _RedBlackLHMap RedBlackLHMap;

struct _RedBlackLHMapIterator {
	struct RedBlackIterator bucket_iter;
	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict last_bucket;
};
typedef struct _RedBlackLHMapIterator RedBlackLHMapIterator;

struct _RedBlackLHMapLIterator {
	RedBlackLHMapIterator map_iter;
	RedBlackLock *restrict map_lock;
};
typedef struct _RedBlackLHMapLIterator RedBlackLHMapLIterator;

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_lhmap_init(RedBlackLHMap *const restrict map);
void
red_black_lhmap_destroy(RedBlackLHMap *const restrict map);

int
red_black_lhmap_lock(RedBlackLHMap *const restrict map);
int
red_black_lhmap_unlock(RedBlackLHMap *const restrict map);

int
red_black_lhmap_insert(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length);
int
red_black_lhmap_insert_u(RedBlackLHMap *const restrict map,
			 const void *const key,
			 const size_t length);

int
red_black_lhmap_update(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length,
		       void **const restrict old_ptr);
int
red_black_lhmap_update_u(RedBlackLHMap *const restrict map,
			 const void *const key,
			 const size_t length,
			 void **const restrict old_ptr);

int
red_black_lhmap_delete(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length);
int
red_black_lhmap_delete_u(RedBlackLHMap *const restrict map,
			 const void *const key,
			 const size_t length);

int
red_black_lhmap_remove(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length,
		       void **const restrict key_ptr);
int
red_black_lhmap_remove_u(RedBlackLHMap *const restrict map,
			 const void *const key,
			 const size_t length,
			 void **const restrict key_ptr);

int
red_black_lhmap_find(RedBlackLHMap *const restrict map,
		     const void *const key,
		     const size_t length);
bool
red_black_lhmap_find_u(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length);

int
red_black_lhmap_fetch(RedBlackLHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict key_ptr);
bool
red_black_lhmap_fetch_u(RedBlackLHMap *const restrict map,
			const void *const key,
			const size_t length,
			void **const restrict key_ptr);

int
red_black_lhmap_count(RedBlackLHMap *const restrict map);
unsigned int
red_black_lhmap_count_u(RedBlackLHMap *const restrict map);

int
red_black_lhmap_verify(RedBlackLHMap *const restrict map);
bool
red_black_lhmap_verify_u(RedBlackLHMap *const restrict map);

void
red_black_lhmap_iterator_init(RedBlackLHMapIterator *const restrict iter,
			      RedBlackLHMap *const restrict map);
bool
red_black_lhmap_iterator_next(RedBlackLHMapIterator *const restrict iter,
			      void **const restrict key_ptr,
			      size_t *const restrict length_ptr);

int
red_black_lhmap_literator_init(RedBlackLHMapLIterator *const restrict iter,
			       RedBlackLHMap *const restrict map);
int
red_black_lhmap_literator_unlock(RedBlackLHMapLIterator *const restrict iter);

int
red_black_lhmap_literator_next(RedBlackLHMapLIterator *const restrict iter,
			       void **const restrict key_ptr,
			       size_t *const restrict length_ptr);

bool
red_black_lhmap_literator_next_u(RedBlackLHMapLIterator *const restrict iter,
				 void **const restrict key_ptr,
				 size_t *const restrict length_ptr);
#endif /* ifndef RED_BLACK_LHMAP_H_ */
