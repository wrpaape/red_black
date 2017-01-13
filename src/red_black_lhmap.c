#include "red_black_lhmap.h"  /* LHMap types */
#include "red_black_hnode.h"  /* HNode|Key, initializer, comparator */
#include "red_black_insert.h" /* red_black_insert */
#include "red_black_update.h" /* red_black_update */
#include "red_black_delete.h" /* red_black_delete */
#include "red_black_remove.h" /* red_black_remove */
#include "red_black_find.h"   /* red_black_find */
#include "red_black_fetch.h"  /* red_black_fetch */
#include "red_black_verify.h" /* red_black_verify */
#include "red_black_concat.h" /* red_black_concat */
#include "red_black_append.h" /* red_black_append */
#include "red_black_malloc.h" /* RED_BLACK_MALLOC|REALLOC|FREE */


/* hash map macros
 * ────────────────────────────────────────────────────────────────────────── */


static inline void
rbhb_init(struct RedBlackLHBucket *const restrict bucket)
{
	RBL_INIT(&bucket->lock);

	bucket->root = NULL;

	rbnf_hnode_factory_init(&bucket->node_factory);
}


static inline void
rbhb_destroy(struct RedBlackLHBucket *const restrict bucket)
{
	RBL_DESTROY(&bucket->lock);

	rbnf_destroy(&bucket->node_factory);
}


static inline void
rbhm_reset_buckets(struct RedBlackLHBucket *const restrict buckets,
		   const unsigned int old_count,
		   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *volatile restrict next;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict bucket_until;
	RedBlackJumpBuffer jump_buffer;

	bucket       = buckets;
	bucket_until = bucket + old_count;

	end_ptr = &head; /* concat 1st list with itself, set head */

	/* traverse old buckets in first half */
	do {
#ifdef OSX
		/* need to completely retouch all realloc'd memory for OSX */
		RBL_INIT(&bucket->lock);
#endif /* ifdef OSX */
		node         = bucket->root;
		bucket->root = NULL; /* remove tree */

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_concat(node,
					   end_ptr);

		/* if don't touch node_factory, lock fails later for OSX*/
		/* reset expansion constant of node_factories */
		rbnf_reset(&bucket->node_factory);

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

	while (1) {
		next = head->left; /* must fetch next before NULLed in append */

		/* fetch hash key hash */
		hash = ((struct RedBlackHNode *) head)->hkey.hash;

		/* fetch new bucket */
		bucket = &buckets[hash & new_count_m1];

		/* append to bucket tree, may jump */
		red_black_append(&bucket->root,
				 &red_black_hkey_comparator,
				 &jump_buffer,
				 head);

NEXT_NODE:
		if (next == NULL)
			return;

		head = next;
	}
}

static inline int
rbhm_expand(RedBlackLHMap *const restrict map)
{
	const unsigned int old_count = map->count.buckets_m1 + 1;
	const unsigned int new_count = old_count * 2;

	/* double bucket capacity */
	struct RedBlackLHBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(struct RedBlackLHBucket) * new_count);

	if (new_buckets == NULL)
		return -1; /* OUT OF MEMORY, old buckets still valid */

	const unsigned int new_count_m1 = new_count - 1;

	map->buckets             = new_buckets;
	map->count.buckets_m1    = new_count_m1;
	map->count.max_capacity *= 2;

	/* take care of initialization of new memory, re-insert nodes */
	rbhm_reset_buckets(new_buckets,
			   old_count,
			   new_count_m1);

	return 1; /* return successful insertion status */
}

int
red_black_lhmap_init(RedBlackLHMap *const restrict map)
{

	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict buckets;
	struct RedBlackLHBucket *restrict bucket_until;

	buckets = RED_BLACK_MALLOC(  sizeof(struct RedBlackLHBucket)
				   * RBHMC_INIT_BUCKET_COUNT);

	if (buckets == NULL)
		return -1;

	bucket       = buckets;
	bucket_until = bucket + RBHMC_INIT_BUCKET_COUNT;

	/* initialize buckets */
	do {
		rbhb_init(bucket);

		++bucket;
	} while (bucket < bucket_until);

	/* initialize lock */
	RBL_INIT(&map->lock);

	map->buckets = buckets;

	red_black_hmap_count_init(&map->count);

	return 0;
}


void
red_black_lhmap_destroy(RedBlackLHMap *const restrict map)
{
	struct RedBlackLHBucket *restrict buckets;
	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict last_bucket;

	/* destroy lock */
	RBL_DESTROY(&map->lock);

	buckets     = map->buckets;
	last_bucket = buckets + map->count.buckets_m1;
	bucket      = buckets;

	/* destroy buckets */
	do {
		rbhb_destroy(bucket);

		++bucket;
	} while (bucket <= last_bucket);

	/* free buckets buffer */
	RED_BLACK_FREE(buckets);
}


int
red_black_lhmap_insert(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		/* 1st entry, obtain an EXCLUSIVE lock on bucket */
		if (RBL_LOCK_WRITE(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(&map->lock);
			return -2;
		}

		/* insert hkey into bucket tree */
		status = red_black_insert(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey); /* 1, 0 */
	} else {
		/* jumped, fetch jump status */
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0, -1 */
	}

	/* release EXCLUSIVE lock on bucket */
	if (RBL_UNLOCK_WRITE(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* if successfully inserted hkey, increment count.entries */
	if (status == 1) {
		/* obtain an EXCLUSIVE lock on map */
		if (RBL_LOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */

		++(map->count.entries);

		/* expand if too many collisions */
		if (map->count.entries > map->count.max_capacity)
			status = rbhm_expand(map); /* 1, -1 */

		/* release EXCLUSIVE lock on map */
		if (RBL_UNLOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}


int
red_black_lhmap_update(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length,
		       void **const restrict old_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict old_hkey_ptr;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		/* 1st entry, obtain an EXCLUSIVE lock on bucket */
		if (RBL_LOCK_WRITE(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(map_lock);
			return -2;
		}

		/* insert hkey into bucket tree */
		status = red_black_update(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey,
					  (void **) &old_hkey_ptr); /* 1, 0 */
	} else {
		/* jumped, fetch jump status */
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0, -1 */
	}

	/* release EXCLUSIVE lock on bucket */
	if (RBL_UNLOCK_WRITE(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	if (status == 0) {
		/* old_hkey_ptr points to old hkey, retrieve key */
		*old_ptr = (void *) old_hkey_ptr->key;

	} else if (status == 1) {
		/* successfully inserted hkey, increment count.entries */
		/* obtain an EXCLUSIVE lock on map */
		if (RBL_LOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */

		++(map->count.entries);

		/* expand if too many collisions */
		if (map->count.entries > map->count.max_capacity)
			status = rbhm_expand(map); /* 1, -1 */

		/* release EXCLUSIVE lock on map */
		if (RBL_UNLOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}


int
red_black_lhmap_delete(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		/* 1st entry, obtain an EXCLUSIVE lock on bucket */
		if (RBL_LOCK_WRITE(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(map_lock);
			return -2;
		}

		/* delete hkey from bucket tree */
		status = red_black_delete(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey); /* 1, 0 */
	} else {
		/* jumped, fetch jump status */
		status = RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */
	}

	/* release EXCLUSIVE lock on bucket */
	if (RBL_UNLOCK_WRITE(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* if successfully deleted hkey, decrement count.entries */
	if (status != 0) {
		/* obtain an EXCLUSIVE lock on map */
		if (RBL_LOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */

		--(map->count.entries);

		/* release EXCLUSIVE lock on map */
		if (RBL_UNLOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}


int
red_black_lhmap_remove(RedBlackLHMap *const restrict map,
		       const void *const key,
		       const size_t length,
		       void **const restrict key_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict hkey_ptr;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		/* 1st entry, obtain an EXCLUSIVE lock on bucket */
		if (RBL_LOCK_WRITE(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(map_lock);
			return -2;
		}

		/* remove hkey from bucket tree */
		status = red_black_remove(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey,
					  (void **) &hkey_ptr); /* 1, 0 */
	} else {
		/* jumped, fetch jump status */
		status = RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */
	}

	/* release EXCLUSIVE lock on bucket */
	if (RBL_UNLOCK_WRITE(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	if (status == 0) {
		/* hkey_ptr points to old hkey, retrieve key */
		*key_ptr = (void *) hkey_ptr->key;

	} else {
		/* successfully removed hkey, decrement count.entries */
		/* obtain an EXCLUSIVE lock on map */
		if (RBL_LOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */

		--(map->count.entries);

		/* release EXCLUSIVE lock on map */
		if (RBL_UNLOCK_WRITE(map_lock) != 0)
			return -2; /* lock failure */
	}

	return status;
}


int
red_black_lhmap_find(RedBlackLHMap *const restrict map,
		     const void *const key,
		     const size_t length)
{
	struct RedBlackHKey hkey;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* printf("count_m1: %u\n", map->count.buckets_m1); */
	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	/* obtain a SHARED lock on bucket */
	if (RBL_LOCK_READ(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* perform binary search on bucket's tree */
	status = (int) red_black_find(bucket->root,
				      &red_black_hkey_comparator,
				      (const void *) &hkey); /* 1, 0 */

	/* release SHARED lock on bucket */
	if (RBL_UNLOCK_READ(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	return status; /* return found status */
}


int
red_black_lhmap_fetch(RedBlackLHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict key_ptr)
{
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict hkey_ptr;
	struct RedBlackLHBucket *restrict bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
				key,
				length);

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	bucket_lock = &bucket->lock;

	/* obtain a SHARED lock on bucket */
	if (RBL_LOCK_READ(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* perform binary search on bucket's tree */
	status = (int) red_black_fetch(bucket->root,
				       &red_black_hkey_comparator,
				       (const void *) &hkey,
				       (void **) &hkey_ptr); /* 1, 0 */

	/* release SHARED lock on bucket */
	if (RBL_UNLOCK_READ(bucket_lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	if (status != 0) /* retrieve key from hkey_ptr */
		*key_ptr = (void *) hkey_ptr->key;

	return status; /* return found status */
}


int
red_black_lhmap_count(RedBlackLHMap *const restrict map)
{
	RedBlackLock *restrict map_lock;
	int count;

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	count = (int) map->count.entries;

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	return count;
}


int
red_black_lhmap_literator_init(RedBlackLHMapLIterator *const restrict iter,
			       RedBlackLHMap *const restrict map)
{
	RedBlackLock *const restrict map_lock = &map->lock;

	/* obtain a SHARED lock on map */
	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	struct RedBlackLHBucket *const restrict first_bucket = map->buckets;

	/* obtain a SHARED lock on bucket */
	if (RBL_LOCK_READ(&first_bucket->lock) != 0) {
		(void) RBL_UNLOCK_READ(map_lock);
		return -2;
	}

	/* initialize first bucket iterator */
	red_black_iterator_init_asc(&iter->bucket_iter,
				    first_bucket->root);

	iter->bucket      = first_bucket;
	iter->last_bucket = first_bucket + map->count.buckets_m1;
	iter->map_lock    = map_lock;

	return 0;
}


int
red_black_lhmap_literator_unlock(RedBlackLHMapLIterator *const restrict iter)
{
	/* release SHARED lock on bucket */
	if (RBL_UNLOCK_READ(&iter->bucket->lock) != 0)
		return -2; /* lock failure */

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(iter->map_lock) != 0)
		return -2; /* lock failure */

	return 0;
}


int
red_black_lhmap_literator_next(RedBlackLHMapLIterator *const restrict iter,
			       void **const restrict key_ptr,
			       size_t *const restrict length_ptr)
{
	struct RedBlackIterator *restrict bucket_iter;
	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict last_bucket;
	struct RedBlackHKey *restrict hkey_ptr;
	RedBlackLock *restrict bucket_lock;

	bucket_iter = &iter->bucket_iter;

	/* if current bucket has remaining keys, return with next key, length */
	if (red_black_iterator_next(bucket_iter,
				    (void **) &hkey_ptr)) {
		*key_ptr    = (void *) hkey_ptr->key;
		*length_ptr = hkey_ptr->length;
		return 1;
	}

	bucket      = iter->bucket;
	last_bucket = iter->last_bucket;

	bucket_lock = &bucket->lock;

	while (1) {
		/* release SHARED lock on bucket */
		if (RBL_UNLOCK_READ(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(iter->map_lock);
			return -2;
		}

		if (bucket >= last_bucket) {
			/* all buckets traversed, release SHARED lock on map */
			if (RBL_UNLOCK_READ(iter->map_lock) != 0)
				return -2;

			return 0;
		}

		++bucket; /* advance to next bucket */

		bucket_lock = &bucket->lock;

		/* obtain a SHARED lock on bucket */
		if (RBL_LOCK_READ(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(iter->map_lock);
			return -2;
		}

		/* re-initialize bucket iterator */
		red_black_iterator_set_asc(bucket_iter,
					   bucket->root);

		/* if bucket is non-empty, return with first key, length */
		if (red_black_iterator_next(bucket_iter,
					    (void **) &hkey_ptr)) {
			iter->bucket = bucket; /* update bucket */

			*key_ptr    = (void *) hkey_ptr->key;
			*length_ptr = hkey_ptr->length;
			return 1;
		}
	}
}


int
red_black_lhmap_verify(RedBlackLHMap *const restrict map)
{
	struct RedBlackLHBucket *restrict bucket;
	struct RedBlackLHBucket *restrict last_bucket;
	RedBlackLock *restrict map_lock;
	RedBlackLock *restrict bucket_lock;
	int status;
	RedBlackJumpBuffer jump_buffer;

	/* obtain a SHARED lock on map */
	map_lock = &map->lock;

	if (RBL_LOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	bucket      = map->buckets;
	last_bucket = bucket + map->count.buckets_m1;

	bucket_lock = &bucket->lock;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	if (status) {
		/* first entry, verify buckets while traversing */
		while (1) {
			/* obtain a SHARED lock on bucket */
			if (RBL_LOCK_READ(bucket_lock) != 0) {
				(void) RBL_UNLOCK_READ(map_lock);
				return -2; /* lock failure */
			}

			status
			= (int) red_black_verify(bucket->root,
						 &red_black_hkey_comparator,
						 &jump_buffer); /* 1, 0 */

			/* release SHARED lock on bucket */
			if (RBL_UNLOCK_READ(bucket_lock) != 0) {
				(void) RBL_UNLOCK_READ(map_lock);
				return -2; /* lock failure */
			}

			if (!status)
				break; /* invalid bucket */

			++bucket;
			if (bucket > last_bucket)
				break; /* finished traversal */

			bucket_lock = &bucket->lock;
		}

	} else {
		/* jumped invalid, release SHARED lock on bucket */
		if (RBL_UNLOCK_READ(bucket_lock) != 0) {
			(void) RBL_UNLOCK_READ(map_lock);
			return -2; /* lock failure */
		}
	}

	/* release SHARED lock on map */
	if (RBL_UNLOCK_READ(map_lock) != 0)
		return -2; /* lock failure */

	return status; /* return valid/invalid (1/0) */
}
