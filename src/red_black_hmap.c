#include "red_black_hmap.h"   /* HMap types */
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


static inline void
rbhb_init(struct RedBlackHBucket *const restrict bucket)
{
	bucket->root = NULL;

	rbnf_init(&bucket->node_factory,
		  &hnode_factory_blueprint);

}


static inline void
rbhb_destroy(struct RedBlackHBucket *const restrict bucket)
{
	rbnf_destroy(&bucket->node_factory);
}


static inline void
rbhm_reset_buckets(struct RedBlackHBucket *const restrict buckets,
		   const unsigned int old_count,
		   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *volatile restrict next;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict bucket_until;
	RedBlackJumpBuffer jump_buffer;

	bucket       = buckets;
	bucket_until = bucket + old_count;

	end_ptr = &head; /* concat 1st list with itself, set head */

	/* traverse old buckets in first half */
	do {
		node         = bucket->root; /* remove tree */
		bucket->root = NULL;

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_concat(node,
					   end_ptr);

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
rbhm_expand(RedBlackHMap *const restrict map)
{
	const unsigned int old_count = map->count.buckets_m1 + 1;
	const unsigned int new_count = old_count * 2;

	/* double bucket capacity */
	struct RedBlackHBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(*new_buckets) * new_count);

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


bool
red_black_hmap_init(RedBlackHMap *const restrict map)
{
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict buckets;
	struct RedBlackHBucket *restrict bucket_until;
	bool status;

	buckets = RED_BLACK_MALLOC(  sizeof(*buckets)
				   * RBHMC_INIT_BUCKET_COUNT);

	status = (buckets != NULL);

	if (status) {
		bucket       = buckets;
		bucket_until = bucket + RBHMC_INIT_BUCKET_COUNT;

		/* initialize buckets */
		do {
			rblhb_init(bucket);

			++bucket;
		} while (bucket < bucket_until);

		map->buckets = buckets;

		red_black_hmap_count_init(&map->count);
	}

	return status;
}


void
red_black_hmap_destroy(RedBlackHMap *const restrict map)
{
	struct RedBlackHBucket *restrict buckets;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;

	buckets     = map->buckets;
	last_bucket = buckets + map->count.buckets_m1;
	bucket      = buckets;

	/* destroy buckets */
	do {
		rblhb_destroy(bucket);

		++bucket;
	} while (bucket <= last_bucket);

	/* free buckets buffer */
	RED_BLACK_FREE(buckets);
}


int
red_black_hmap_insert(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* IF 0 -> 1st entry, insert hkey into bucket tree
	 * ELSE -> jumped, fetch jump status */
	if (status == 0)
		status = red_black_insert(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey); /* 1, 0 */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map); /* 1, -1 */

	return status; /* 1, 0, -1 */
}


int
red_black_hmap_update(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict old_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict old_hkey_ptr;
	struct RedBlackHBucket *restrict bucket;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* IF 0 -> 1st entry, insert/swap hkey into bucket tree
	 * ELSE -> jumped, fetch jump status */
	if (status == 0)
		status = red_black_update(&bucket->root,
					  &red_black_hkey_comparator,
					  &bucket->node_factory,
					  &jump_buffer,
					  (const void *) &hkey,
					  (void **) &old_hkey_ptr); /* 1, 0 */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* return early to avoid extra conditional */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */


	if (status == 0) {
		/* old_hkey_ptr points to old hkey, retrieve key */
		*old_ptr = (void *) old_hkey_ptr->key;

	} else {
		/* successfully inserted hkey, increment count.entries */
		++(map->count.entries);

		/* expand if too many collisions */
		if (map->count.entries > map->count.max_capacity)
			status = rbhm_expand(map); /* 1, -1 */

	}

	return status;
}


int
red_black_hmap_delete(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* IF 0 -> 1st entry, delete hkey from bucket tree
	 * ELSE -> jumped, fetch jump status */
	status = (status == 0)
	       ? red_black_delete(&bucket->root,
				  &red_black_hkey_comparator,
				  &bucket->node_factory,
				  &jump_buffer,
				  (const void *) &hkey) /* 1, 0 */
	       : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	map->count.entries -= status; /* decrement 1 or 0 */

	return status;
}


int
red_black_hmap_remove(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict key_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict hkey_ptr;
	struct RedBlackHBucket *restrict bucket;
	int status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* IF 0 -> 1st entry, remove hkey from bucket tree
	 * ELSE -> jumped, fetch jump status */
	status = (status == 0)
	       ? red_black_remove(&bucket->root,
				  &red_black_hkey_comparator,
				  &bucket->node_factory,
				  &jump_buffer,
				  (const void *) &hkey,
				  (void **) &hkey_ptr) /* 1, 0 */
	       : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	if (status == 0) /* hkey_ptr points to old hkey, retrieve key */
		*key_ptr = (void *) hkey_ptr->key;
	else /* successfully removed hkey, decrement count.entries */
		--(map->count.entries);

	return status;
}


bool
red_black_hmap_find(RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length)
{
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* perform binary search on bucket's tree */
	return red_black_find(bucket->root,
			      &red_black_hkey_comparator,
			      (const void *) &hkey); /* true, false */
}


bool
red_black_hmap_fetch(RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict key_ptr)
{
	struct RedBlackHKey hkey;
	struct RedBlackHKey *restrict hkey_ptr;
	struct RedBlackHBucket *restrict bucket;
	bool status;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* perform binary search on bucket's tree */
	status = red_black_fetch(bucket->root,
				 &red_black_hkey_comparator,
				 (const void *) &hkey,
				 (void **) &hkey_ptr); /* true, false */

	if (status) /* retrieve key from hkey_ptr */
		*key_ptr = (void *) hkey_ptr->key;

	return status; /* return found status */
}


unsigned int
red_black_hmap_count(RedBlackHMap *const restrict map)
{
	return map->count.entries;
}


bool
red_black_hmap_verify(RedBlackHMap *const restrict map)
{
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;
	bool status;
	RedBlackJumpBuffer jump_buffer;

	bucket      = map->buckets;
	last_bucket = bucket + map->count.buckets_m1;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	/* IF 0 -> first entry, verify buckets while traversing */
	/* ELSE -> jumped invalid, return */
	if (status)
		do {
			status = red_black_verify(bucket->root,
						  &red_black_hkey_comparator,
						  &jump_buffer);

			if (!status)
				break; /* invalid bucket */

			++bucket;
		} while (bucket <= last_bucket);

	return status; /* return valid/invalid (true/false) */
}


void
red_black_hmap_iterator_init(RedBlackHMapIterator *const restrict iter,
			     RedBlackHMap *const restrict map)
{
	struct RedBlackHBucket *restrict first_bucket;

	first_bucket = map->buckets;

	/* initialize first bucket iterator */
	red_black_asc_iterator_init(&iter->bucket_iter,
				    first_bucket->root);

	iter->bucket      = first_bucket;
	iter->last_bucket = first_bucket + map->count.buckets_m1;
}


bool
red_black_hmap_iterator_next(RedBlackHMapIterator *const restrict iter,
			     void **const restrict key_ptr,
			     size_t *const restrict length_ptr)
{
	struct RedBlackIterator *restrict bucket_iter;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;
	struct RedBlackHKey *restrict hkey_ptr;

	bucket_iter = &iter->bucket_iter;

	/* if current bucket has remaining keys, return with next key, length */
	if (red_black_iterator_next(bucket_iter,
				    (void **) &hkey_ptr))
		goto FOUND_NEXT;

	bucket      = iter->bucket;
	last_bucket = iter->last_bucket;

	while (bucket < last_bucket) {
		++bucket; /* advance to next bucket */

		/* reset bucket iterator */
		red_black_asc_iterator_set(bucket_iter,
					   bucket->root);

		/* if bucket is non-empty, return with first key, length */
		if (red_black_iterator_next(bucket_iter,
					    (void **) &hkey_ptr)) {
			iter->bucket = bucket; /* update bucket */
FOUND_NEXT:
			*key_ptr    = (void *) hkey_ptr->key;
			*length_ptr = hkey_ptr->length;
			return true;
		}
	}

	return false; /* all buckets traversed, no keys left */
}
