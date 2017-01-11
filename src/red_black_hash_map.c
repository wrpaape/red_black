#include "red_black_hash_map.h" /* HashMap types */
#include "red_black_insert.h"	/* red_black_insert */
#include "red_black_update.h"	/* red_black_update */
#include "red_black_delete.h"	/* red_black_delete */
#include "red_black_remove.h"	/* red_black_remove */
#include "red_black_find.h"	/* red_black_find */
#include "red_black_fetch.h"	/* red_black_fetch */
#include "red_black_verify.h"	/* red_black_verify */
#include "red_black_flatten.h"  /* red_black_flatten */
#include "red_black_append.h"   /* red_black_append */

/* hash map macros
 * ────────────────────────────────────────────────────────────────────────── */
/* initial bucket count, MUST be non-zero multiple of 2 */
#define RBHM_INIT_BUCKET_COUNT	8

/* maximum average collision count per bucket without requiring expansion */
#define RBHM_MAX_AVG_COLLISIONS	8


static inline void
rbhb_init(struct RedBlackHashBucket *const restrict bucket)
{
	RED_BLACK_LOCK_INIT(&bucket->lock);

	bucket->root = NULL;

	hash_node_allocator_init(&bucket->allocator);
}

static inline void
rbhb_destroy(struct RedBlackHashBucket *const restrict bucket)
{
	RED_BLACK_LOCK_DESTROY(&bucket->lock);

	rba_destroy(&bucket->allocator);
}

static inline void
rbhb_reset(struct RedBlackHashBucket *const restrict buckets,
	   const unsigned int old_count,
	   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	struct RedBlackHashBucket *restrict bucket;
	struct RedBlackHashBucket *restrict bucket_until;
	RedBlackJumpBuffer jump_buffer;

	bucket       = buckets;
	bucket_until = bucket + old_count;

	end_ptr = &head; /* concat 1st list with itself, set head */

	/* traverse old buckets in first half */
	do {
		node         = bucket->root;
		bucket->root = NULL; /* remove tree */

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_concat(node,
					   end_ptr);

		/* reset expansion constant of allocators */
		hash_node_allocator_reset(&bucket->allocator);

		++bucket;
	} while (bucket < bucket_until);

	bucket_until = bucket + old_count;

	/* initialize newly allocated second half */
	do {
		rbhb_init(bucket);

		++bucket;
	} while (bucket < bucket_until);


	/* dump node list into empty hash table */
	if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
		goto NEXT_NODE;

	do {
		/* fetch hash key hash */
		hash = ((struct RedBlackHashNode *) head)->hash_key.hash;

		/* fetch new bucket */
		bucket = &buckets[hash & new_count_m1];

		/* append to bucket tree, may jump */
		red_black_append(&bucket->root,
				 &red_black_hash_key_comparator,
				 &jump_buffer,
				 head);
NEXT_NODE:
		head = head->left;
	} while (head != NULL);
}


static inline int
rbhm_expand(struct RedBlackHashMap *const restrict map)
{
	const unsigned int old_count = map->count.buckets_m1 + 1;
	const unsigned int new_count = old_count * 2;

	struct RedBlackHashBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(struct RedBlackHashBucket) * new_count);

	if (new_buckets == NULL)
		return -1; /* old buckets still valid */

	const unsigned int new_count_m1 = new_count - 1;

	map->buckets             = new_buckets;
	map->count.buckets_m1    = new_count_m1;
	map->count.max_capacity *= 2;

	/* take care of initialization of new memory, re-insert nodes */
	rbhb_reset(new_buckets,
		   old_count,
		   new_count_m1);

	return 1;
}

int
red_black_hash_map_init(RedBlackHashMap *const restrict map)
{
	RED_BLACK_LOCK_INIT(&map->lock);

	map->buckets = RED_BLACK_MALLOC(sizeof(struct RedBlackHashBucket)
					* RBHM_INIT_BUCKET_COUNT);

	if (map->buckets == NULL)
		return -1;

	map->count.buckets_m1 = RBHM_INIT_BUCKET_COUNT - 1;
	map->count.entries    = 0;
	map->max_capacity     = RBHM_INIT_BUCKET_COUNT
			      * RBHM_MAX_AVG_COLLISIONS;
}

void
red_black_hash_map_destroy(RedBlackHashMap *const restrict map)
{
	struct RedBlackHashBucket *restrict buckets;
	struct RedBlackHashBucket *restrict bucket;
	struct RedBlackHashBucket *restrict last_bucket;

	/* destroy lock */
	RED_BLACK_LOCK_DESTROY(&map->lock);

	buckets     = map->buckets;
	last_bucket = buckets + map->count.buckets_m1;
	bucket      = buckets;

	/* destroy buckets */
	do {
		rbhb_destroy(bucket);

		++bucket;
	} while (bucket <= last_bucket);

	/* free bucket buffer */
	RED_BLACK_FREE(buckets);
}


int
red_black_hash_map_insert(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHashKey hash_key;
	struct RedBlackHashBucket *restrict bucket;
	int status;

	/* initialize hash key */
	red_black_hash_key_init(&hash_key,
				key,
				length);

	/* obtain a SHARED lock on map */
	if (RED_BLACK_LOCK_READ(&map->lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hash_key.hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		/* 1st entry, obtain an EXCLUSIVE lock on bucket */
		if (RED_BLACK_LOCK_WRITE(&bucket->lock) != 0) {
			(void) RED_BLACK_UNLOCK(&map->lock);
			return -2;
		}

		/* insert hash_key into bucket tree */
		status = red_black_insert(&bucket->root,
					  &red_black_hash_key_comparator,
					  &bucket->allocator,
					  &jump_buffer,
					  (const void *) &hash_key); /* 1, 0 */
	} else {
		/* jumped, fetch jump status */
		status = RED_BLACK_JUMP_3_STATUS(status);
	}

	/* release EXCLUSIVE lock on bucket */
	if (RED_BLACK_UNLOCK(&bucket->lock) != 0) {
		(void) RED_BLACK_UNLOCK(&map->lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RED_BLACK_UNLOCK(&map->lock) != 0)
		return -2; /* lock failure */


	/* if successfully inserted hash_key, update count.entries */
	if (status == 1) {
		/* obtain an EXCLUSIVE lock on map */
		if (RED_BLACK_LOCK_WRITE(&map->lock) != 0)
			return -2; /* lock failure */

		++(map->count.entries);

		/* expand if too many collisions */
		if (map->count.entries > map->count.max_capacity)
			status = rbhm_expand(map); /* 1, -1 */

		/* release EXCLUSIVE lock on map */
		if (RED_BLACK_UNLOCK(&map->lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}
