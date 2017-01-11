#ifndef RED_BLACK_HASH_MAP_H_
#define RED_BLACK_HASH_MAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_lock.h"      /* RedBlackLock */
#include "red_black_allocator.h" /* RedBlackAllocator */


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
	RedBlackLock *restrict map_lock;
	const struct RedBlackHashBucket *restrict bucket;
	const struct RedBlackHashBucket *restrict bucket_until;
	struct RedBlackIterator bucket_iterator;
};

typedef struct _RedBlackHashMapIterator RedBlackHashMapIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hash_map_init(RedBlackHashMap *const restrict map);

int
red_black_hash_map_insert(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length);


#endif /* ifndef RED_BLACK_HASH_MAP_H_ */
