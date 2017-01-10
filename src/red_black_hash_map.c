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

#define FETCH_BUCKET(BUCKETS, HASH, COUNT_BUCKETS_M1)

/* static inline int */
/* rbhb_insert(struct RedBlackHashBucket *const restrict bucket, */
/* 	    const struct RedBlackHashKey *const restrict key) */
/* { */
/* 	RedBlackJumpBuffer jump_buffer; */
/* 	int status; */

/* 	status = RED_BLACK_SET_JUMP(jump_buffer); */

/* 	if (status == 0) { */
/* 		if (RED_BLACK_LOCK_WRITE(&buffer->lock) != 0) */
/* 			return -2; */

/* 		status = red_black_insert(&bucket->root, */
/* 					  &red_black_hash_key_comparator, */
/* 					  &bucket->allocator, */
/* 					  &jump_buffer, */
/* 					  key) /1* 1, 0 *1/ */
/* 	} else { */
/* 		status = RED_BLACK_JUMP_3_STATUS(status); */
/* 	} */

/* 	return (RED_BLACK_UNLOCK(&buffer->lock) == 0) */
/* 	     ? status */
/* 	     : -2; */
/* } */

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

	/* gather nodes in first half, reset allocator expand constants */
	do {
		node         = bucket->root;
		bucket->root = NULL; /* remove tree */

		end_ptr = red_black_concat(node,
					   end_ptr);

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


int
red_black_hash_map_init(RedBlackHashMap *const restrict map)
{
}


static inline struct RedBlackHashBucket *
rbhm_fetch_bucket(const struct RedBlackHashMap *const restrict map,
		  const struct RedBlackHashKey *const restrict key)
{
	return &map->buckets[key->hash & map->count.buckets_m1];
}

static inline void
rbhm_do_expand(struct RedBlackHashBucket *restrict old_bucket,
	       const unsigned int count_old,
	       struct RedBlackHashBucket *const restrict new_buckets,
	       const unsigned int new_buckets_m1)
{
	       const struct RedBlackHashBucket *restrict old_bucket_until,
}

static inline int
rbhm_expand(struct RedBlackHashMap *const restrict map)
{
	const unsigned int count_old = map->count.buckets_m1 + 1;
	const unsigned int count_new = count_old * 2;

	struct RedBlackHashBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(struct RedBlackHashBucket) * count_new);

	if ()


	RED_BLACK_FREE(old_buckets);
}

int
red_black_hash_map_insert(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length)
{
	struct RedBlackHashKey hash_key;
	struct RedBlackHashBucket *restrict bucket;
	int status;

	red_black_hash_key_init(&hash_key,
				key,
				length);

	if (RED_BLACK_LOCK_READ(&map->lock) != 0)
		return -2; /* lock failure */

	bucket = rbhm_fetch_bucket(map,
				   hash_key);

	status = red_black_hash_bucket_insert(bucket,
					      hash_key);

	if (RED_BLACK_UNLOCK(&map->lock) != 0)
		return -2; /* lock failure */

	if (status == 1) {
		if (RED_BLACK_LOCK_WRITE(&map->lock) != 0)
			return -2; /* lock failure */

		++(map->count.entries);

		if (map->count.entries > map->count.max_capacity)
			status = rbhm_expand(map);

		if (RED_BLACK_UNLOCK(&map->lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}
