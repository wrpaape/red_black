#ifndef RED_BLACK_HASH_MAP_H_
#define RED_BLACK_HASH_MAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_lock.h"      /* RedBlackLock */
#include "red_black_allocator.h" /* RedBlackAllocator */
#include "red_black_iterator.h"	 /* RedBlackIterator */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHashBucket {
	RedBlackLock lock;
	struct RedBlackNode *restrict root;
	struct RedBlackAllocator allocator;
};

struct RedBlackHashMapCount {
	unsigned int buckets_m1;
	unsigned int entries;
	unsigned int max_capacity;
};

struct _RedBlackHashMap {
	RedBlackLock lock;
	struct RedBlackHashBucket *restrict buckets;
	struct RedBlackHashMapCount count;
};

typedef struct _RedBlackHashMap RedBlackHashMap;


struct _RedBlackHashMapIterator {
	struct RedBlackIterator bucket_iterator;
	struct RedBlackHashBucket *restrict bucket;
	struct RedBlackHashBucket *restrict last_bucket;
	RedBlackLock *restrict map_lock;
};

typedef struct _RedBlackHashMapIterator RedBlackHashMapIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hash_map_init(RedBlackHashMap *const restrict map);

void
red_black_hash_map_destroy(RedBlackHashMap *const restrict map);

int
red_black_hash_map_insert(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length);

int
red_black_hash_map_update(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict old_ptr);

int
red_black_hash_map_delete(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length);

int
red_black_hash_map_remove(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict key_ptr);

int
red_black_hash_map_find(RedBlackHashMap *const restrict map,
			const void *const key,
			const size_t length);

int
red_black_hash_map_fetch(RedBlackHashMap *const restrict map,
			 const void *const key,
			 const size_t length,
			 void **const restrict key_ptr);

int
red_black_hash_map_count(RedBlackHashMap *const restrict map);

int
red_black_hash_map_iterator_init(RedBlackHashMapIterator *const restrict iterator,
				 RedBlackHashMap *const restrict map);
int
red_black_hash_map_iterator_bail(RedBlackHashMapIterator *const restrict iterator);

int
red_black_hash_map_iterator_next(RedBlackHashMapIterator *const restrict iterator,
				 void **const restrict key_ptr);

int
red_black_hash_map_verify(RedBlackHashMap *const restrict map);

#endif /* ifndef RED_BLACK_HASH_MAP_H_ */
