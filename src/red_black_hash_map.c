#include "red_black_hash_map.h"


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

static inline int
rbhm_expand_map(struct RedBlackHashMap *const restrict map)
{
	struct RedBlackIterator iterator;
	RedBlackJumpBuffer jump_buffer;
	int status;

	const struct RedBlackHashKey *restrict hash_key;

	struct RedBlackHashBucket *restrict new_bucket;
	struct RedBlackHashBucket *restrict old_bucket;
	struct RedBlackHashBucket *restrict old_buckets;
	struct RedBlackHashBucket *restrict last_old_bucket;

	old_buckets     = map->buckets;
	last_old_bucket = old_buckets + map->count.buckets_m1;

	old_bucket = old_buckets;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE) /* will NEVER jump FALSE */
		goto NEXT_BUCKET;

	if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* OUT OF MEMORY */

	do {
		red_black_iterator_init_asc(&iterator,
					    old_bucket->root);

		while (red_black_iterator_next(&iterator,
					       (const void **) &hash_key)) {

			new_bucket = rbhm_fetch_bucket(map,
						       hash_key);

			(void) red_black_insert(&new_bucket->root,
						&red_black_hash_key_comparator,
						&new_bucket->allocator,
						&jump_buffer,
						(const void *) hash_key);
		}
NEXT_BUCKET:
		rba_destroy(&bucket->allocator);
		++old_bucket;
	} while (old_bucket <= last_old_bucket);


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

		if (map->count.entries > map->count.expand)
			status = rbhm_expand_map(map);

		if (RED_BLACK_UNLOCK(&map->lock) != 0)
			return -2; /* lock failure */
	}


	return status;
}
