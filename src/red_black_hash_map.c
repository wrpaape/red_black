#include "red_black_hash_map.h"


int
red_black_hash_map_init(RedBlackHashMap *const restrict map)
{
}


int
red_black_hash_map_insert(RedBlackHashMap *const restrict map,
			  const void *const key,
			  const size_t length)
{
	struct RedBlackHashKey hash_key;
	struct RedBlackHashBucket *restrict bucket;
	unsigned int count_entries;
	int status;

	red_black_hash_key_init(&hash_key,
				key,
				length);

	if (RED_BLACK_LOCK_READ(&map->lock) == 0) {
		red_black


	}

	return -2; /* lock failure */
}
