#ifndef RED_BLACK_HASH_MAP_H_
#define RED_BLACK_HASH_MAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hash_bucket.h" /* HashBucket, Lock, MALLOC/FREE */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHashMapCount {
	unsigned int buckets_m1;
	unsigned int entries;
	unsigned int shrink;
	unsigned int expand;
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
