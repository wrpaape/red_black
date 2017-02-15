#include "red_black_hmap.h"			 /* HMap, HMapItor, HNode */
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
#include "red_black_hmap/red_black_hcongruent.h" /* red_black_hcongruent */
#include "red_black_hmap/red_black_hverify.h"    /* red_black_hverify */
#include "red_black_hmap/red_black_hcopy.h"      /* red_black_hcopy */
#include "red_black_hmap/red_black_hconcat.h"    /* red_black_hconcat */
#include "red_black_common/red_black_malloc.h"   /* MALLOC|REALLOC|FREE */



/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* initial bucket count, MUST be power of 2 (and at LEAST 2) */
#define RBHM_INIT_BUCKET_COUNT	8
/* maximum average collision count per bucket without requiring expansion */
#define RBHM_MAX_AVG_COLLISIONS 8
/* initial expansion factor (left shift of bucket count), must be > 0 */
#define RBHM_INIT_EXPAND_FACTOR	1
/* increment applied to expand factor on each expand (double->quadruple->...) */
#define RBHM_EXPAND_FACTOR_INC	1


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
	} while (bucket <= last_new_bucket);

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
		red_black_hadd(&bucket->root,
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
	const unsigned int old_count	     = map->count.buckets_m1 + 1;
	const unsigned int old_expand_factor = map->expand_factor;
	const unsigned int new_count	     = old_count << old_expand_factor;

	/* double bucket capacity */
	struct RedBlackHBucket *const restrict new_buckets
	= RED_BLACK_REALLOC(map->buckets,
			    sizeof(*new_buckets) * new_count);

	if (new_buckets == NULL)
		return -1; /* OUT OF MEMORY, old buckets still valid */

	const unsigned int new_count_m1 = new_count - 1;

	map->buckets              = new_buckets;
	map->count.buckets_m1     = new_count_m1;
	map->count.max_capacity <<= old_expand_factor;
	map->expand_factor        = old_expand_factor + RBHM_EXPAND_FACTOR_INC;

	/* take care of initialization of new memory, re-insert nodes */
	rbhm_reset_buckets(new_buckets,
			   old_count,
			   new_count_m1);

	return 1; /* return successful insertion status */
}

/* inline expansion, return only on error */
#define RBHM_EXPAND(MAP)						\
do {									\
	const unsigned int old_count = MAP->count.buckets_m1 + 1;	\
	const unsigned int old_expand_factor = MAP->expand_factor;	\
	const unsigned int new_count = old_count << old_expand_factor;	\
	struct RedBlackHBucket *const restrict new_buckets		\
	= RED_BLACK_REALLOC(MAP->buckets,				\
			    sizeof(*new_buckets) * new_count);		\
	if (new_buckets == NULL)					\
		return -1;						\
	const unsigned int new_count_m1 = new_count - 1;		\
	MAP->buckets		  = new_buckets;			\
	MAP->count.buckets_m1     = new_count_m1;			\
	MAP->count.max_capacity <<= old_expand_factor;			\
	MAP->expand_factor = old_expand_factor + RBHM_EXPAND_FACTOR_INC;\
	rbhm_reset_buckets(new_buckets,					\
			   old_count,					\
			   new_count_m1);				\
} while (0)


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

		map->count.buckets_m1   = RBHM_INIT_BUCKET_COUNT - 1;
		map->count.entries      = 0;
		map->count.max_capacity = RBHM_INIT_BUCKET_COUNT
					* RBHM_MAX_AVG_COLLISIONS;

		map->expand_factor = RBHM_INIT_EXPAND_FACTOR;
	}

	return status;
}


bool
red_black_hmap_clone(RedBlackHMap *const restrict dst_map,
		     const RedBlackHMap *const restrict src_map)
{
	struct RedBlackHNode *restrict buffer;
	struct RedBlackHBucket *restrict dst_buckets;
	struct RedBlackHBucket *restrict dst_bucket;
	struct RedBlackHBucket *restrict last_dst_bucket;
	struct RedBlackHBucket *restrict src_bucket;
	unsigned int count_entries;
	unsigned int count_buckets_m1;

	count_buckets_m1 = src_map->count.buckets_m1;

	dst_buckets = RED_BLACK_MALLOC(  sizeof(*dst_buckets)
				       * (count_buckets_m1 + 1));

	if (dst_buckets == NULL)
		return false;

	last_dst_bucket = dst_buckets + count_buckets_m1;
	dst_bucket      = dst_buckets;

	count_entries = src_map->count.entries;

	if (count_entries == 0) {
		/* finish empty initialization */
		do {
			rbhb_init(dst_bucket);

			++dst_bucket;
		} while (dst_bucket <= last_dst_bucket);

	} else {
		/* copy src_map buckets,
		 * allocate nodes from first bucket factory */
		buffer = rbhnf_init_w_nodes(&dst_bucket->factory,
					    count_entries);

		if (buffer == NULL) {
			RED_BLACK_FREE(dst_buckets);
			return false;
		}

		src_bucket = src_map->buckets;

		buffer = red_black_hcopy(&dst_bucket->root,
					 src_bucket->root,
					 buffer);

		do {
			++src_bucket;
			++dst_bucket;

			buffer
			= red_black_hcopy(&dst_bucket->root,
					  src_bucket->root,
					  buffer);

			rbhnf_init(&dst_bucket->factory);
		} while (dst_bucket < last_dst_bucket);
	}

	dst_map->buckets	    = dst_buckets;
	dst_map->count.buckets_m1   = count_buckets_m1;
	dst_map->count.entries      = count_entries;
	dst_map->count.max_capacity = src_map->count.max_capacity;
	dst_map->expand_factor      = src_map->expand_factor;

	return true;
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


static inline bool
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


static inline bool
rb_hmap_similar_walk_both(const struct RedBlackHBucket *restrict bucket1,
			  const struct RedBlackHBucket *restrict bucket2,
			  const unsigned int count_buckets_m1)
{
	struct RedBlackHItor bucket_itor1;
	struct RedBlackHItor bucket_itor2;
	const struct RedBlackHBucket *restrict last_bucket1;
	const struct RedBlackHKey *restrict hkey1;
	const struct RedBlackHKey *restrict hkey2;

	red_black_hitor_init(&bucket_itor2,
			     bucket2->root);

	red_black_hitor_init(&bucket_itor1,
			     bucket1->root);

	last_bucket1 = bucket1 + count_buckets_m1;

	while (1) {
		while (1) {
			hkey1 = red_black_hitor_next_hkey(&bucket_itor1);

			if (hkey1 != NULL)
				break;

			if (bucket1 == last_bucket1)
				return true; /* all hkeys equal */

			++bucket1;

			red_black_hitor_reset(&bucket_itor1,
					      bucket1->root);
		}


		while (1) {
			hkey2 = red_black_hitor_next_hkey(&bucket_itor2);

			if (hkey2 != NULL)
				break;

			++bucket2; /* no need to check for last */

			red_black_hitor_reset(&bucket_itor2,
					      bucket2->root);
		}

		if (red_black_hkey_comparator(hkey1,
					      hkey2) != 0)
			return false;
	}
}

static inline bool
rb_hmap_similar_walk_find(const struct RedBlackHBucket *restrict buckets1,
			  const struct RedBlackHBucket *restrict buckets2,
			  const unsigned int count_buckets1_m1,
			  const unsigned int count_buckets2_m1)
{
	struct RedBlackHItor walk_bucket_itor;
	const struct RedBlackHBucket *restrict walk_bucket;
	const struct RedBlackHBucket *restrict last_walk_bucket;
	const struct RedBlackHBucket *restrict find_buckets;
	const struct RedBlackHBucket *restrict find_bucket;
	unsigned int count_find_buckets_m1;
	const struct RedBlackHKey *restrict hkey;

	/* ensure buckets being traversed are more densly packed than those
	 * being searched */
	if (count_buckets1_m1 < count_buckets2_m1) {
		walk_bucket	 = buckets1;
		last_walk_bucket = walk_bucket + count_buckets1_m1;

		find_buckets          = buckets2;
		count_find_buckets_m1 = count_buckets2_m1;

	} else {
		walk_bucket	 = buckets2;
		last_walk_bucket = walk_bucket + count_buckets2_m1;

		find_buckets          = buckets1;
		count_find_buckets_m1 = count_buckets1_m1;
	}

	red_black_hitor_init(&walk_bucket_itor,
			     walk_bucket->root);

	do {
		while (1) {
			hkey = red_black_hitor_next_hkey(&walk_bucket_itor);

			if (hkey != NULL)
				break;

			if (walk_bucket == last_walk_bucket)
				return true; /* found all hkeys */

			++walk_bucket;

			red_black_hitor_reset(&walk_bucket_itor,
					      walk_bucket->root);
		}

		/* fetch bucket */
		find_bucket = &find_buckets[hkey->hash & count_find_buckets_m1];

	} while (red_black_hfind(find_bucket->root,
				 hkey));

	return false; /* failed to find hkey */
}

bool
red_black_hmap_similar(const RedBlackHMap *const map1,
		       const RedBlackHMap *const map2)
{
	bool status;
	struct RedBlackHBucket *restrict buckets1;
	struct RedBlackHBucket *restrict buckets2;
	unsigned int count_buckets1_m1;
	unsigned int count_buckets2_m1;

	status = (map1 == map2);

	if (!status) {
		status = (map1->count.entries == map2->count.entries);

		if (status) {
			buckets1	  = map1->buckets;
			count_buckets1_m1 = map1->count.buckets_m1;

			buckets2	  = map2->buckets;
			count_buckets2_m1 = map2->count.buckets_m1;

			/* if both maps have same number of buckets, walk in
			 * parallel for O(N) comparisons, otherwise must walk
			 * one and find from the other (O(NlogN) comparisons) */
			status = (count_buckets1_m1 == count_buckets2_m1)
			       ? rb_hmap_similar_walk_both(buckets1,
							   buckets2,
							   count_buckets1_m1)
			       : rb_hmap_similar_walk_find(buckets1,
							   buckets2,
							   count_buckets1_m1,
							   count_buckets2_m1);
		}
	}

	return status;
}


int
red_black_hmap_insert_all(RedBlackHMap *const restrict dst_map,
			  const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHBucket *restrict dst_bucket;
	const struct RedBlackHBucket *restrict src_bucket;
	const struct RedBlackHBucket *restrict last_src_bucket;
	int status;


	src_bucket      = src_map->buckets;
	last_src_bucket = src_bucket + src_map->count.buckets_m1;

	red_black_hitor_init(&src_bucket_itor,
			     src_bucket->root);

	const unsigned int init_dst_count_entries = dst_map->count.entries;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE) {
		++(dst_map->count.entries);
		goto CHECK_EXPAND;

	} else if (status == RED_BLACK_JUMP_VALUE_3_ERROR) {
		return -1;
	}

	while (1) {
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			/* done, return difference in count */
			if (src_bucket == last_src_bucket)
				return (int) (  dst_map->count.entries
					      - init_dst_count_entries);

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      src_bucket->root);
		}

		/* fetch bucket */
		dst_bucket
		= &dst_map->buckets[src_hkey->hash & dst_map->count.buckets_m1];

		status = red_black_hinsert(&dst_bucket->root,
					   &dst_bucket->factory,
					   jump_buffer,
					   src_hkey); /* 1, 0 */

		dst_map->count.entries += status;
CHECK_EXPAND:
		if (dst_map->count.entries > dst_map->count.max_capacity)
			RBHM_EXPAND(dst_map);
	}

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
