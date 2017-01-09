#ifndef RED_BLACK_HASH_MAP_H_
#define RED_BLACK_HASH_MAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hash_bucket.h" /* HashBucket, Lock, MALLOC/FREE */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackHashMap {
	RedBlackLock lock;
	struct RedBlackHashBucket *restrict buckets;
	unsigned int count_buckets;
	unsigned int count_entries;
};

typedef struct _RedBlackHashMap RedBlackHashMap;


struct _RedBlackHashMapIterator {
	RedBlackLock *restrict map_lock;
	const struct RedBlackHashBucket *restrict bucket;
	struct RedBlackIterator *restrict bucket_iterator;
};

typedef struct _RedBlackHashMapIterator RedBlackHashMapIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hash_map_init(RedBlackHashMap *const restrict map,
			const RedBlackComparator comparator);

#endif /* ifndef RED_BLACK_HASH_MAP_H_ */
