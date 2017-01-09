#ifndef RED_BLACK_HASH_BUCKET_H_
#define RED_BLACK_HASH_BUCKET_H_

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


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hash_bucket_init(struct RedBlackHashBucket *const restrict bucket);

int
red_black_hash_bucket_insert(struct RedBlackHashBucket *const restrict bucket,
			     const struct RedBlackHashKey *const restrict key);


#endif /* ifndef RED_BLACK_HASH_BUCKET_H_ */
