#include "red_black_hmap/red_black_hmap.h"       /* HMap, HMapItor */
#include "red_black_hmap/red_black_hnode.h"      /* HNode|Key, init, cmptor */
#include "red_black_hmap/red_black_hinsert.h"    /* red_black_hinsert */
#include "red_black_hmap/red_black_hput.h"       /* red_black_hput */
#include "red_black_hmap/red_black_hupdate.h"    /* red_black_hupdate */
#include "red_black_hmap/red_black_hadd.h"       /* red_black_hadd */
#include "red_black_hmap/red_black_hdelete.h"    /* red_black_hdelete */
#include "red_black_hmap/red_black_hremove.h"    /* red_black_hremove */
#include "red_black_hmap/red_black_hdrop.h"      /* red_black_hdrop */
#include "red_black_hmap/red_black_hpluck.h"     /* red_black_hpluck */
#include "red_black_hmap/red_black_hfind.h"      /* red_black_hfind */
#include "red_black_hmap/red_black_hfetch.h"     /* red_black_hfetch */
#include "red_black_hmap/red_black_hreplace.h"   /* red_black_hreplace */
#include "red_black_hmap/red_black_hexchange.h"  /* red_black_hexchange */
#include "red_black_hmap/red_black_hget.h"       /* red_black_hget */
#include "red_black_hmap/red_black_hset.h"       /* red_black_hset */
#include "red_black_hmap/red_black_hswap.h"      /* red_black_hswap */
#include "red_black_tree/red_black_hcongruent.h" /* red_black_hcongruent */
#include "red_black_hmap/red_black_hverify.h"    /* red_black_hverify */
#include "red_black_hmap/red_black_hconcat.h"    /* red_black_hconcat */
#include "red_black_common/red_black_malloc.h"   /* MALLOC|REALLOC|FREE */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* initial bucket count, MUST be power of 2 */
#define RBHM_INIT_BUCKET_COUNT	8
/* maximum average collision count per bucket without requiring expansion */
#define RBHM_MAX_AVG_COLLISIONS 8
#if RED_BLACK_HMAP_VAR_EXPAND
/* initial expansion factor (left shift of bucket count), must be > 0 */
#define RBHM_INIT_EXPAND_FACTOR 1
#endif /* if RED_BLACK_HMAP_VAR_EXPAND */


static inline void
rbhb_init(struct RedBlackHBucket *const restrict bucket)
{
	bucket->root = NULL;

	rbhnf_init(&bucket->factory);
}


static inline void
rbhb_destroy(struct RedBlackHBucket *const restrict bucket)
{
	rbhnf_destroy(&bucket->factory);
}


static inline void
rbhm_reset_buckets(struct RedBlackHBucket *const restrict buckets,
		   const unsigned int old_count,
		   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *volatile restrict next;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict first_new_bucket;
	struct RedBlackHBucket *restrict last_new_bucket;
	RedBlackJumpBuffer jump_buffer;

	first_new_bucket = buckets + old_count;
	last_new_bucket  = buckets + new_count_m1;
	bucket           = buckets;

	end_ptr = &head; /* concat 1st list with itself, set head */

	/* traverse old buckets in old portion */
	do {
		node         = bucket->root; /* remove tree */
		bucket->root = NULL;

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_hconcat(node,
					    end_ptr);

		/* reset expansion constant of node_factories */
		rbhnf_reset(&bucket->factory);

		++bucket;
	} while (bucket < first_new_bucket);

	/* initialize newly allocated portion */
	do {
		rbhb_init(bucket);

		++bucket;
	} while (bucket < last_new_bucket);

	/* dump node list into empty hash table */
	if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
		goto NEXT_NODE;

	while (1) {
		/* fetch hash key hash */
		hash = head->hkey.hash;

		/* must fetch next before NULLed in add */
		next = head->left;

		/* fetch new bucket */
		bucket = &buckets[hash & new_count_m1];

		/* add to bucket tree, may jump */
		red_black_add(&bucket->root,
			      &red_black_hkey_comparator,
			      jump_buffer,
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
#if RED_BLACK_HMAP_VAR_EXPAND
	const unsigned int old_expand_factor = map->expand_factor;

	++(map->expand_factor);

	const unsigned int new_count = old_count << old_expand_factor;
#else
	const unsigned int new_count = old_count * 2;
#endif /* if RED_BLACK_HMAP_VAR_EXPAND */

	/* double bucket capacity */
	struct RedBlackHBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(*new_buckets) * new_count);

	if (new_buckets == NULL)
		return -1; /* OUT OF MEMORY, old buckets still valid */

	const unsigned int new_count_m1 = new_count - 1;

	map->buckets             = new_buckets;
	map->count.buckets_m1    = new_count_m1;
#if RED_BLACK_HMAP_VAR_EXPAND
	map->count.max_capacity <<= old_expand_factor;
#else
	map->count.max_capacity *= 2;
#endif /* if RED_BLACK_HMAP_VAR_EXPAND */

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
				   * RBHM_INIT_BUCKET_COUNT);

	status = (buckets != NULL);

	if (status) {
		bucket       = buckets;
		bucket_until = bucket + RBHM_INIT_BUCKET_COUNT;

		/* initialize buckets */
		do {
			rbhb_init(bucket);

			++bucket;
		} while (bucket < bucket_until);

		map->buckets = buckets;

		red_black_hmap_count_init(&map->count);

		map->count.buckets_m1   = RBHM_INIT_BUCKET_COUNT - 1;
		map->count.entries      = 0;
		map->count.max_capacity = RBHM_INIT_BUCKET_COUNT
					* RBHM_MAX_AVG_COLLISIONS;
#if RED_BLACK_HMAP_VAR_EXPAND
		map->expand_factor = RBHM_INIT_EXPAND_FACTOR;
#endif /* if RED_BLACK_HMAP_VAR_EXPAND */
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
		rbhb_destroy(bucket);

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
		status = red_black_hinsert(&bucket->root,
					   &bucket->factory,
					   jump_buffer,
					   &hkey); /* 1, 0 */
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
red_black_hmap_put(RedBlackHMap *const restrict map,
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
		status = red_black_hput(&bucket->root,
					&bucket->factory,
					jump_buffer,
					&hkey); /* 1, 0 */
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
		status = red_black_hupdate(&bucket->root,
					   &bucket->factory,
					   jump_buffer,
					   &hkey,
					   old_ptr); /* 1, 0 */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map); /* 1, -1 */

	return status;
}


bool
red_black_hmap_add(RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length)
{
	RedBlackHash hash;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict *restrict tree;
	struct RedBlackHBucket *restrict bucket;
	int status;

	/* hash key */
	hash = red_black_hkey_hash((const unsigned char *) key,
				   length);

	/* fetch bucket */
	bucket = &map->buckets[hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0) {
		status = (status == RED_BLACK_JUMP_VALUE_3_TRUE); /* 1, 0 */

		map->count.entries += status; /* if no ERROR, increment */

		return (bool) status;
	}

	tree = &bucket->root;

	node = rbhnf_allocate(&bucket->factory,
			      jump_buffer);

	/* init HNode's HKey */
	node->hkey.hash   = hash;
	node->hkey.key    = (const unsigned char *) key;
	node->hkey.length = length;

	red_black_hadd(tree,
		       jump_buffer,
		       node);

	/* if returned, successful addition */
	++(map->count.entries);

	return true;
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
	       ? red_black_hdelete(&bucket->root,
				   &bucket->factory,
				   jump_buffer,
				   &hkey) /* 1, 0 */
	       : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	map->count.entries -= status; /* decrement 1 or 0 */

	return status;
}


int
red_black_hmap_remove(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict remove_ptr)
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

	/* IF 0 -> 1st entry, remove hkey from bucket tree
	 * ELSE -> jumped, fetch jump status */
	status = (status == 0)
	       ? red_black_hremove(&bucket->root,
				   &bucket->factory,
				   jump_buffer,
				   &hkey,
				   remove_ptr) /* 1, 0 */
	       : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	map->count.entries -= status; /* decrement 1 or 0 */

	return status;
}


void
red_black_hmap_drop(RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* decrement count entries (guaranteed deletion) */
	--(map->count.entries);

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_hdrop(&bucket->root,
				&bucket->factory,
				jump_buffer,
				&hkey);
}


void *
red_black_hmap_pluck(RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	void *volatile plucked_key;
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* decrement count entries (guaranteed deletion) */
	--(map->count.entries);

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_hpluck(&bucket->root,
				 &bucket->factory,
				 jump_buffer,
				 &hkey,
				 (void **) &plucked_key);

	return plucked_key;
}



bool
red_black_hmap_find(const RedBlackHMap *const restrict map,
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
	return red_black_hfind(bucket->root,
			       &hkey); /* true, false */
}


bool
red_black_hmap_fetch(const RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict fetch_ptr)
{
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* attempt to get key of node with matching hkey */
	return red_black_hfetch(bucket->root,
				&hkey,
				fetch_ptr);
}


bool
red_black_hmap_replace(const RedBlackHMap *const restrict map,
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

	/* attempt to set node with matching hkey's key */
	return red_black_hreplace(bucket->root,
				  &hkey);
}


bool
red_black_hmap_exchange(const RedBlackHMap *const restrict map,
			const void *const key,
			const size_t length,
			void **const restrict old_ptr)
{
	struct RedBlackHKey hkey;
	struct RedBlackHBucket *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* attempt to swap key with node having matching hkey */
	return red_black_hexchange(bucket->root,
				   &hkey,
				   old_ptr);
}


void *
red_black_hmap_get(const RedBlackHMap *const restrict map,
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

	/* get key of node having matching hkey */
	return red_black_hget(bucket->root,
			      &hkey);
}


void
red_black_hmap_set(const RedBlackHMap *const restrict map,
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

	/* set key of node having matching hkey */
	red_black_hset(bucket->root,
		       &hkey);
}


void *
red_black_hmap_swap(const RedBlackHMap *const restrict map,
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

	/* swap keys with node having matching hkey */
	return red_black_hswap(bucket->root,
			       &hkey);
}

bool
red_black_hmap_empty(RedBlackHMap *const restrict map)
{
	return (map->count.entries == 0);
}


unsigned int
red_black_hmap_count(const RedBlackHMap *const restrict map)
{
	return map->count.entries;
}


bool
red_black_hmap_verify(const RedBlackHMap *const restrict map)
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
			red_black_hverify(bucket->root,
					  jump_buffer);

			++bucket;
		} while (bucket <= last_bucket);

	return status; /* return valid/invalid (true/false) */
}


bool
red_black_hmap_congruent(const RedBlackHMap *const map1,
			 const RedBlackHMap *const map2)
{
	bool status;

	status = (map1 == map2);

	if (!status) {
		const unsigned int count_buckets_m1 = map1->count.buckets_m1;

		status = (count_buckets_m1     == map2->count.buckets_m1)
		      && (map1->count.entries  == map2->count.entries)
		      && rb_hmap_congruent(map1,
					   map2,
					   count_buckets_m1);
	}

	return status;
}

bool
rb_hmap_congruent(const RedBlackHMap *const restrict map1,
		  const RedBlackHMap *const restrict map2,
		  const unsigned int count_buckets_m1)
{
	struct RedBlackHBucket *restrict bucket1;
	struct RedBlackHBucket *restrict bucket2;
	struct RedBlackHBucket *restrict last_bucket1;
	bool status;
	RedBlackJumpBuffer jump_buffer;

	bucket1      = map1->buckets;
	last_bucket1 = bucket1 + count_buckets_m1;

	bucket2 = map2->buckets;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	/* IF 0 -> first entry, verify buckets while traversing */
	/* ELSE -> jumped invalid, return */
	if (status)
		while (1) {
			red_black_hcongruent(bucket1->root,
					     bucket2->root,
					     jump_buffer);

			if (++bucket1 > last_bucket1)
				break; /* done */

			++bucket2;
		}

	return status; /* return valid/invalid (true/false) */
}


bool
red_black_hmap_similar(const RedBlackHMap *const map1,
		       const RedBlackHMap *const map2)
{
	bool status;

	status = (map1 == map2);

	if (!status) {
		const unsigned int count_entries = map1->count_entries;

		status = (count_entries == map2->count.buckets_m1)
		      && rb_hmap_similar(map1,
					 map2,
					 count_entries);
	}

	return status;
}


bool
rb_hmap_similar(const RedBlackHMap *const restrict map1,
		const RedBlackHMap *const restrict map2,
		const unsigned int count_entries)
{
	struct RedBlackHItor bucket_itor1;
	struct RedBlackHItor bucket_itor2;

	return false;
}


void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
			 RedBlackHMap *const restrict map)
{
	struct RedBlackHBucket *restrict first_bucket;

	first_bucket = map->buckets;

	/* initialize first bucket itor */
	red_black_hitor_init(&itor->bucket_itor,
			     first_bucket->root);

	itor->bucket      = first_bucket;
	itor->last_bucket = first_bucket + map->count.buckets_m1;
}


bool
red_black_hmap_itor_next(RedBlackHMapItor *const restrict itor,
			 void **const restrict key_ptr,
			 size_t *const restrict length_ptr)
{
	struct RedBlackHItor *restrict bucket_itor;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;

	bucket_itor = &itor->bucket_itor;

	/* if current bucket has remaining keys, return with next key, length */
	if (red_black_hitor_next(bucket_itor,
				 key_ptr,
				 length_ptr))
		return true;

	bucket      = itor->bucket;
	last_bucket = itor->last_bucket;

	while (bucket < last_bucket) {
		++bucket; /* advance to next bucket */

		/* reset bucket itor */
		red_black_hitor_reset(bucket_itor,
				      bucket->root);

		/* if bucket is non-empty, return with first key, length */
		if (red_black_hitor_next(bucket_itor,
					 key_ptr,
					 length_ptr)) {
			itor->bucket = bucket; /* update bucket */
			return true;
		}
	}

	return false; /* all buckets traversed, no keys left */
}
