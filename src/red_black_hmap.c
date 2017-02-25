#include "red_black_hmap.h"			  /* HMap, HMapItor, HNode */
#include "red_black_hmap/red_black_hinsert.h"     /* red_black_hinsert */
#include "red_black_hmap/red_black_hput.h"        /* red_black_hput */
#include "red_black_hmap/red_black_hupdate_set.h" /* red_black_hupdate_set */
#include "red_black_hmap/red_black_hupdate_get.h" /* red_black_hupdate_get */
#include "red_black_hmap/red_black_hadd.h"        /* red_black_hadd */
#include "red_black_hmap/red_black_hdelete.h"     /* red_black_hdelete */
#include "red_black_hmap/red_black_hremove.h"     /* red_black_hremove */
#include "red_black_hmap/red_black_hdrop.h"       /* red_black_hdrop */
#include "red_black_hmap/red_black_hpluck.h"      /* red_black_hpluck */
#include "red_black_hmap/red_black_hfind.h"       /* red_black_hfind */
#include "red_black_hmap/red_black_hfetch.h"      /* red_black_hfetch */
#include "red_black_hmap/red_black_hreplace.h"    /* red_black_hreplace */
#include "red_black_hmap/red_black_hexchange.h"   /* red_black_hexchange */
#include "red_black_hmap/red_black_hget.h"        /* red_black_hget */
#include "red_black_hmap/red_black_hset.h"        /* red_black_hset */
#include "red_black_hmap/red_black_hswap.h"       /* red_black_hswap */
#include "red_black_hmap/red_black_hcongruent.h"  /* red_black_hcongruent */
#include "red_black_hmap/red_black_hverify.h"     /* red_black_hverify */
#include "red_black_hmap/red_black_hcopy.h"       /* red_black_hcopy */
#include "red_black_hmap/red_black_hconcat.h"     /* red_black_hconcat */
#include "red_black_common/red_black_malloc.h"    /* C|REALLOC|FREE */
#include <limits.h>				  /* CHAR_BIT */




/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* initial bucket count, MUST be power of 2 (and at LEAST 2) */
#define RBHM_INIT_BUCKET_COUNT	8
/* maximum average collision count per bucket without requiring expansion */
#define RBHM_MAX_AVG_COLLISIONS 8
/* initial (minimum) maximum capacity of entries before expansion is needed */
#define RBHM_INIT_MAX_CAPACITY	(  RBHM_INIT_BUCKET_COUNT		\
				 * RBHM_MAX_AVG_COLLISIONS)
/* initial expansion factor (left shift of bucket count), must be > 0 */
#define RBHM_INIT_EXPAND_FACTOR	1
/* increment applied to expand factor on each expand (double->quadruple->...) */
#define RBHM_EXPAND_FACTOR_INC	1


/* bit count of unsigned integer, used for bitwise rounding operations */
#define UINT_BIT (CHAR_BIT * sizeof(unsigned int))

/* 'COUNT' must be (a) greater than 0 and (b) less than 'INT_MAX':
 *	(a) rounding is only done in cases of expansion
 *	(b) hash maps with over 'INT_MAX' entries are documented as having
 *	    ~~UnDeFiNeD bEhAvIoR~~ */
#define NEXT_POWER_OF_TWO(COUNT) (1 << (UINT_BIT - __builtin_clz(COUNT)))




static inline void
rbhm_reset_buckets(struct RedBlackHNode *restrict *const restrict buckets,
		   const unsigned int old_count_m1,
		   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *volatile restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHNode *restrict *restrict bucket;
	struct RedBlackHNode *restrict *restrict last_old_bucket;
	struct RedBlackHNode *restrict *restrict last_new_bucket;
	RedBlackJumpBuffer jump_buffer;

	last_old_bucket = buckets + old_count_m1;
	last_new_bucket = buckets + new_count_m1;
	bucket          = buckets;

	/* concat 1st list with itself, set head */
	end_ptr = (struct RedBlackHNode *restrict *restrict) &head;

	/* traverse old buckets in old portion */
	do {
		node    = *bucket; /* remove tree */
		*bucket = NULL;

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_hconcat(node,
					    end_ptr);

		++bucket;
	} while (bucket <= last_old_bucket);

	/* initialize newly allocated portion */
	do {
		*bucket++ = NULL;
	} while (bucket <= last_new_bucket);

	/* dump node list into empty hash table */
	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		node = head;

		if (node == NULL)
			return;

		/* fetch hash key hash */
		hash = node->hkey.hash;

		/* must fetch next head before NULLed in add */
		head = node->left;

		/* fetch new bucket */
		bucket = &buckets[hash & new_count_m1];

		/* add to bucket tree, may jump */
		red_black_hadd(bucket,
			       jump_buffer,
			       node);
	}
}


static inline int
rbhm_expand(RedBlackHMap *const restrict map,
	    const int failure_status)
{
	const unsigned int old_count_m1      = map->count.buckets_m1;
	const unsigned int old_expand_factor = map->expand_factor;

	const unsigned int new_count = (old_count_m1 + 1) << old_expand_factor;

	/* reallocate buckets buffer */
	struct RedBlackHNode *restrict *const restrict new_buckets
	= RED_BLACK_REALLOC((void *) map->buckets,
			    sizeof(*new_buckets) * new_count);

	if (new_buckets == NULL)
		return failure_status; /* old buckets still valid */

	const unsigned int new_count_m1 = new_count - 1;

	map->buckets              = new_buckets;
	map->count.buckets_m1     = new_count_m1;
	map->count.max_capacity <<= old_expand_factor;
	map->expand_factor        = old_expand_factor + RBHM_EXPAND_FACTOR_INC;

	/* take care of initialization of new memory, re-insert nodes */
	rbhm_reset_buckets(new_buckets,
			   old_count_m1,
			   new_count_m1);

	return 1; /* return successful insertion status */
}


bool
red_black_hmap_init(RedBlackHMap *const restrict map)
{
	struct RedBlackHNode *restrict *restrict buckets;
	bool status;

	buckets = RED_BLACK_CALLOC(RBHM_INIT_BUCKET_COUNT,
				   sizeof(*buckets));

	status = (buckets != NULL);

	if (status) {
		map->buckets		= buckets;
		map->count.buckets_m1   = RBHM_INIT_BUCKET_COUNT - 1;
		map->count.entries      = 0;
		map->count.max_capacity = RBHM_INIT_MAX_CAPACITY;
		map->expand_factor	= RBHM_INIT_EXPAND_FACTOR;

		rbhnf_init(&map->factory);
	}

	return status;
}


bool
red_black_hmap_clone(RedBlackHMap *const restrict dst_map,
		     const RedBlackHMap *const restrict src_map)
{
	struct RedBlackHNode *restrict buffer;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNode *restrict *restrict last_dst_bucket;
	struct RedBlackHNode *restrict *restrict src_bucket;
	unsigned int count_entries;
	unsigned int count_buckets_m1;

	count_buckets_m1 = src_map->count.buckets_m1;

	dst_bucket = RED_BLACK_CALLOC(count_buckets_m1 + 1,
				      sizeof(dst_bucket));

	if (dst_bucket == NULL)
		return false;

	count_entries = src_map->count.entries;

	dst_map->buckets	    = dst_bucket;
	dst_map->count.buckets_m1   = count_buckets_m1;
	dst_map->count.entries      = count_entries;
	dst_map->count.max_capacity = src_map->count.max_capacity;
	dst_map->expand_factor      = RBHM_INIT_EXPAND_FACTOR;

	if (count_entries == 0) {
		/* finish empty initialization */
		rbhnf_init(&dst_map->factory);

	} else {
		/* copy src_map buckets,
		 * allocate nodes from first bucket factory */
		buffer = rbhnf_init_w_nodes(&dst_map->factory,
					    count_entries);

		if (buffer == NULL) {
			RED_BLACK_FREE((void *) dst_bucket);
			return false;
		}

		last_dst_bucket = dst_bucket + count_buckets_m1;

		src_bucket = src_map->buckets;

		while (1) {
			buffer = red_black_hcopy(dst_bucket,
						 *src_bucket,
						 buffer);
			++dst_bucket;

			if (dst_bucket > last_dst_bucket)
				break;

			++src_bucket;
		}
	}

	return true;
}


void
red_black_hmap_destroy(RedBlackHMap *const restrict map)
{

	RED_BLACK_FREE((void *) map->buckets); /* free buckets buffer */

	rbhnf_destroy(&map->factory); /* destroy factory */
}


int
red_black_hmap_insert(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict *restrict bucket;
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
		status = red_black_hinsert(bucket,
					   &map->factory,
					   jump_buffer,
					   &hkey); /* 1, 0 */
	else if (status < 0)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map,
				     -1); /* 1, -1 */

	return status; /* 1, 0, -1 */
}


int
red_black_hmap_put(RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict *restrict bucket;
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
		status = red_black_hput(bucket,
					&map->factory,
					jump_buffer,
					&hkey); /* 1, 0 */
	else if (status < 0)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map,
				     -1); /* 1, -1 */

	return status; /* 1, 0, -1 */
}


int
red_black_hmap_update_set(RedBlackHMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict old_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict *restrict bucket;
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
		status = red_black_hupdate_set(bucket,
					       &map->factory,
					       jump_buffer,
					       &hkey,
					       old_ptr); /* 1, 0 */
	else if (status < 0)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map,
				     -1); /* 1, -1 */

	return status;
}


int
red_black_hmap_update_get(RedBlackHMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict old_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict *restrict bucket;
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
		status = red_black_hupdate_get(bucket,
					       &map->factory,
					       jump_buffer,
					       &hkey,
					       old_ptr); /* 1, 0 */
	else if (status < 0)
		return -1; /* return early to avoid decrementing count */
	else
		status = RED_BLACK_JUMP_3_STATUS(status); /* 1, 0 */

	map->count.entries += status; /* add 1 or 0 */

	/* expand if too many collisions */
	if (map->count.entries > map->count.max_capacity)
		status = rbhm_expand(map,
				     -1); /* 1, -1 */

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
	struct RedBlackHNode *restrict *restrict bucket;
	int status;

	/* hash key */
	hash = red_black_hkey_hash((const unsigned char *) key,
				   length);

	/* fetch bucket */
	bucket = &map->buckets[hash & map->count.buckets_m1];

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0) {
		/* increment count entires if no RED_BLACK_MALLOC error */
		status = (status > 0);

		map->count.entries += status;

		return (bool) status;
	}

	node = rbhnf_allocate(&map->factory,
			      jump_buffer);

	/* init HNode's HKey */
	node->hkey.hash   = hash;
	node->hkey.key    = (const unsigned char *) key;
	node->hkey.length = length;

	red_black_hadd(bucket,
		       jump_buffer,
		       node);

	/* if returned, successful addition */
	++(map->count.entries);

	return (map->count.entries <= map->count.max_capacity)
	    || ((bool) rbhm_expand(map,
				   0));

}


int
red_black_hmap_delete(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict *restrict bucket;
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
	       ? red_black_hdelete(bucket,
				   &map->factory,
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
	struct RedBlackHNode *restrict *restrict bucket;
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
	       ? red_black_hremove(bucket,
				   &map->factory,
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
	struct RedBlackHNode *restrict *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* decrement count entries (guaranteed deletion) */
	--(map->count.entries);

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_hdrop(bucket,
				&map->factory,
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
	struct RedBlackHNode *restrict *restrict bucket;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket */
	bucket = &map->buckets[hkey.hash & map->count.buckets_m1];

	/* decrement count entries (guaranteed deletion) */
	--(map->count.entries);

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_hpluck(bucket,
				 &map->factory,
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
	const struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* perform binary search on bucket's tree */
	return red_black_hfind(bucket_root,
			       &hkey); /* true, false */
}


bool
red_black_hmap_fetch(const RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict fetch_ptr)
{
	struct RedBlackHKey hkey;
	const struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* attempt to get key of node with matching hkey */
	return red_black_hfetch(bucket_root,
				&hkey,
				fetch_ptr);
}


bool
red_black_hmap_replace(const RedBlackHMap *const restrict map,
		       const void *const key,
		       const size_t length)
{
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* attempt to set node with matching hkey's key */
	return red_black_hreplace(bucket_root,
				  &hkey);
}


bool
red_black_hmap_exchange(const RedBlackHMap *const restrict map,
			const void *const key,
			const size_t length,
			void **const restrict old_ptr)
{
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* attempt to swap key with node having matching hkey */
	return red_black_hexchange(bucket_root,
				   &hkey,
				   old_ptr);
}


void *
red_black_hmap_get(const RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length)
{
	struct RedBlackHKey hkey;
	const struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* get key of node having matching hkey */
	return red_black_hget(bucket_root,
			      &hkey);
}


void
red_black_hmap_set(const RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length)
{
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* set key of node having matching hkey */
	red_black_hset(bucket_root,
		       &hkey);
}


void *
red_black_hmap_swap(const RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length)
{
	struct RedBlackHKey hkey;
	struct RedBlackHNode *restrict bucket_root;

	/* initialize hash key */
	red_black_hkey_init(&hkey,
			    key,
			    length);

	/* fetch bucket root */
	bucket_root = map->buckets[hkey.hash & map->count.buckets_m1];

	/* swap keys with node having matching hkey */
	return red_black_hswap(bucket_root,
			       &hkey);
}


bool
red_black_hmap_empty(const RedBlackHMap *const restrict map)
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
	struct RedBlackHNode *const restrict *restrict bucket;
	struct RedBlackHNode *const restrict *restrict last_bucket;
	RedBlackJumpBuffer jump_buffer;

	bucket      = map->buckets;
	last_bucket = bucket + map->count.buckets_m1;

	/* IF 0 -> first entry, verify buckets while traversing */
	/* ELSE -> jumped invalid, return */
	if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
		return false;

	do {
		red_black_hverify(*bucket,
				  jump_buffer);

		++bucket;
	} while (bucket <= last_bucket);

	return true; /* passed the gauntlet */
}


static inline bool
rbhm_congruent(const RedBlackHMap *const restrict map1,
	       const RedBlackHMap *const restrict map2,
	       const unsigned int count_buckets_m1)
{
	struct RedBlackHNode *restrict *restrict bucket1;
	struct RedBlackHNode *restrict *restrict bucket2;
	struct RedBlackHNode *restrict *restrict last_bucket1;
	RedBlackJumpBuffer jump_buffer;

	bucket1      = map1->buckets;
	last_bucket1 = bucket1 + count_buckets_m1;

	bucket2 = map2->buckets;

	/* IF 0 -> first entry, verify buckets while traversing */
	/* ELSE -> jumped invalid, return */
	if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
		return false;

	while (1) {
		red_black_hcongruent(*bucket1,
				     *bucket2,
				     jump_buffer);

		++bucket1;

		if (bucket1 > last_bucket1)
			return true; /* passed the gauntlet */

		++bucket2;
	}
}

bool
red_black_hmap_congruent(const RedBlackHMap *const map1,
			 const RedBlackHMap *const map2)
{
	bool status;

	status = (map1 == map2);

	if (!status) {
		const unsigned int count_buckets_m1 = map1->count.buckets_m1;

		status = (count_buckets_m1    == map2->count.buckets_m1)
		      && (map1->count.entries == map2->count.entries)
		      && rbhm_congruent(map1,
					map2,
					count_buckets_m1);
	}

	return status;
}


static inline bool
rbhm_similar_walk(struct RedBlackHNode *const restrict *restrict bucket1,
		  struct RedBlackHNode *const restrict *restrict bucket2,
		  const unsigned int count_buckets_m1)
{
	struct RedBlackHItor bucket_itor1;
	struct RedBlackHItor bucket_itor2;
	struct RedBlackHNode *const restrict *restrict last_bucket1;
	const struct RedBlackHKey *restrict hkey1;
	const struct RedBlackHKey *restrict hkey2;

	red_black_hitor_init(&bucket_itor2,
			     *bucket2);

	red_black_hitor_init(&bucket_itor1,
			     *bucket1);
	last_bucket1 = bucket1 + count_buckets_m1;

	while (1) {
		/* fetch next hkeys, sequence of hkeys must match exactly if
		 * maps are similar and have same number of buckets */
		hkey1 = red_black_hitor_next_hkey(&bucket_itor1);
		hkey2 = red_black_hitor_next_hkey(&bucket_itor2);

		if (hkey1 == NULL) {
			if (hkey2 != NULL)
				return false; /* out of sync */

			if (bucket1 == last_bucket1)
				return true; /* done, all hkeys matched */

			/* advance to next buckets */
			++bucket1;
			red_black_hitor_reset(&bucket_itor1,
					      *bucket1);
			++bucket2;
			red_black_hitor_reset(&bucket_itor2,
					      *bucket2);

		} else if (   (hkey2 == NULL)
			   || (red_black_hkey_comparator(hkey1,
							 hkey2) != 0)) {
			return false; /* out of sync */
		}
	}
}

static inline bool
rbhm_similar_find(struct RedBlackHNode *const restrict *restrict buckets1,
		  struct RedBlackHNode *const restrict *restrict buckets2,
		  const unsigned int count_buckets1_m1,
		  const unsigned int count_buckets2_m1)
{
	struct RedBlackHItor walk_bucket_itor;
	struct RedBlackHNode *const restrict *restrict walk_bucket;
	struct RedBlackHNode *const restrict *restrict last_walk_bucket;
	struct RedBlackHNode *const restrict *restrict find_buckets;
	const struct RedBlackHNode *restrict find_bucket_root;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_find_buckets_m1;

	/* walk over more-densly-packed map, search in less-densly-packed map */
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
			     *walk_bucket);

	do {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&walk_bucket_itor);

			if (hkey != NULL)
				break;

			if (walk_bucket == last_walk_bucket)
				return true; /* found all hkeys */

			++walk_bucket;

			red_black_hitor_reset(&walk_bucket_itor,
					      *walk_bucket);
		}

		/* fetch bucket root */
		find_bucket_root
		= find_buckets[hkey->hash & count_find_buckets_m1];

	} while (red_black_hfind(find_bucket_root,
				 hkey));

	return false; /* failed to find hkey */
}

bool
red_black_hmap_similar(const RedBlackHMap *const map1,
		       const RedBlackHMap *const map2)
{
	bool status;
	struct RedBlackHNode *const restrict *restrict buckets1;
	struct RedBlackHNode *const restrict *restrict buckets2;
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
			       ? rbhm_similar_walk(buckets1,
						   buckets2,
						   count_buckets1_m1)
			       : rbhm_similar_find(buckets1,
						   buckets2,
						   count_buckets1_m1,
						   count_buckets2_m1);
		}
	}

	return status;
}


bool
red_black_hmap_intersect(const RedBlackHMap *const map1,
			 const RedBlackHMap *const map2)
{
	const RedBlackHMap *restrict walk_map;
	const RedBlackHMap *restrict find_map;
	struct RedBlackHItor walk_bucket_itor;
	struct RedBlackHNode *const restrict *restrict walk_bucket;
	struct RedBlackHNode *const restrict *restrict last_walk_bucket;
	struct RedBlackHNode *const restrict *restrict find_buckets;
	const struct RedBlackHNode *restrict find_bucket_root;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_find_buckets_m1;
	struct RedBlackHNode *const restrict *restrict buckets1;
	struct RedBlackHNode *const restrict *restrict buckets2;
	unsigned int count_buckets1_m1;
	unsigned int count_buckets2_m1;

	if (map1 == map2)
		return true; /* all maps intersect with themselves */

	buckets1	  = map1->buckets;
	count_buckets1_m1 = map1->count.buckets_m1;

	buckets2	  = map2->buckets;
	count_buckets2_m1 = map2->count.buckets_m1;

	/* walk over smaller map, search in larger map */
	if (map1->count.entries < map2->count.entries) {
		walk_map = map1;
		find_map = map2;

	} else {
		walk_map = map2;
		find_map = map1;
	}

	walk_bucket	 = walk_map->buckets;
	last_walk_bucket = walk_bucket + walk_map->count.buckets_m1;

	find_buckets	      = find_map->buckets;
	count_find_buckets_m1 = find_map->count.buckets_m1;

	red_black_hitor_init(&walk_bucket_itor,
			     *walk_bucket);

	do {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&walk_bucket_itor);

			if (hkey != NULL)
				break;

			if (walk_bucket == last_walk_bucket)
				return false; /* no intersection found */

			++walk_bucket;

			red_black_hitor_reset(&walk_bucket_itor,
					      *walk_bucket);
		}

		/* fetch bucket root */
		find_bucket_root
		= find_buckets[hkey->hash & count_find_buckets_m1];

	} while (!red_black_hfind(find_bucket_root,
				  hkey));

	return true; /* found an intersection */
}


bool
red_black_hmap_subset(const RedBlackHMap *const map1,
		      const RedBlackHMap *const map2)
{
	struct RedBlackHItor bucket_itor2;
	struct RedBlackHNode *const restrict *restrict buckets1;
	struct RedBlackHNode *const restrict *restrict bucket2;
	struct RedBlackHNode *const restrict *restrict last_bucket2;
	const struct RedBlackHNode *restrict bucket1_root;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_buckets1_m1;

	if (map1 == map2)
		return true; /* all maps subsets of themselves */

	if (map2->count.entries > map1->count.entries)
		return false; /* larger maps never subsets of smaller maps */

	buckets1	  = map1->buckets;
	count_buckets1_m1 = map1->count.buckets_m1;

	bucket2		  = map2->buckets;
	last_bucket2      = bucket2 + map2->count.buckets_m1;

	red_black_hitor_init(&bucket_itor2,
			     *bucket2);

	do {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor2);

			if (hkey != NULL)
				break;

			if (bucket2 == last_bucket2)
				return true; /* all hkeys of map2 in map1 */

			++bucket2;

			red_black_hitor_reset(&bucket_itor2,
					      *bucket2);
		}

		/* fetch bucket root */
		bucket1_root = buckets1[hkey->hash & count_buckets1_m1];

	} while (red_black_hfind(bucket1_root,
				 hkey));

	return false; /* map1 does not contain map2's 'hkey' */
}


static inline int
rbhm_add_list(RedBlackHMap *const restrict map,
	      struct RedBlackHNode *restrict *restrict buckets,
	      unsigned int count_buckets_m1,
	      struct RedBlackHNodeFactory *const restrict factory_ptr,
	      struct RedBlackHNode *volatile restrict head,
	      struct RedBlackHNode *restrict *restrict end_ptr,
	      const unsigned int length)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict *restrict bucket;
	struct RedBlackHNode *restrict *restrict last_old_bucket;
	struct RedBlackHNode *restrict *restrict last_new_bucket;
	RedBlackHash hash;
	unsigned int count_entries;
	unsigned int count_max_capacity;
	unsigned int count_buckets;

	if (length == 0)
		return 0; /* avoid bus error for unset head */

	count_entries	   = map->count.entries + length;
	count_max_capacity = map->count.max_capacity;

	if (count_entries > count_max_capacity) {
		/* need to expand */
		count_max_capacity = NEXT_POWER_OF_TWO(count_entries);
		count_buckets      = count_max_capacity
				   / RBHM_MAX_AVG_COLLISIONS;

		buckets = RED_BLACK_REALLOC((void *) buckets,
					    sizeof(*buckets) * count_buckets);

		if (buckets == NULL) {
			rbhnf_destroy(factory_ptr); /* destroy list */
			return -1; /* RED_BLACK_MALLOC_ERROR */
		}

		/* expand */
		last_old_bucket  = buckets + count_buckets_m1;
		count_buckets_m1 = count_buckets - 1; /* update */
		last_new_bucket  = buckets + count_buckets_m1;
		bucket           = buckets;

		/* traverse old buckets in old portion */
		do {
			node    = *bucket; /* remove tree */
			*bucket = NULL;

			/* append old nodes to input list */
			end_ptr = red_black_hconcat(node,
						    end_ptr);

			++bucket;
		} while (bucket <= last_old_bucket);

		/* initialize newly allocated portion */
		do {
			*bucket++ = NULL;
		} while (bucket <= last_new_bucket);

		/* update rest of map */
		map->buckets             = buckets;
		map->count.buckets_m1    = count_buckets_m1;
		map->count.max_capacity  = count_max_capacity;
		map->expand_factor      += RBHM_EXPAND_FACTOR_INC;
	}

	/* update count entries */
	map->count.entries = count_entries;

	/* terminate list of entries */
	*end_ptr = NULL;

	/* dump list nodes into map */
	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		node = head;

		if (node == NULL)
			return (int) length; /* successfully added 'length' */

		hash = node->hkey.hash;

		head = node->left;

		bucket = &buckets[hash & count_buckets_m1];

		red_black_hadd(bucket,
			       jump_buffer,
			       node);
	}
}


int
red_black_hmap_insert_all(RedBlackHMap *const restrict dst_map,
			  const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict *restrict dst_buckets;
	struct RedBlackHNode *restrict dst_bucket_root;
	struct RedBlackHNodeFactory *restrict dst_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict *restrict end_ptr;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	unsigned int count_new_entries;
	unsigned int dst_count_buckets_m1;
	int status;

	dst_buckets          = dst_map->buckets;
	dst_count_buckets_m1 = dst_map->count.buckets_m1;
	dst_factory_ptr      = &dst_map->factory;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0) {
		rbhnf_destroy(dst_factory_ptr); /* destroy list */
		return -1; /* RED_BLACK_MALLOC ERROR */
	}

	factory_buffer_ptr = &dst_factory_ptr->buffer;

	src_bucket      = src_map->buckets;
	last_src_bucket = src_bucket + src_map->count.buckets_m1;

	red_black_hitor_init(&src_bucket_itor,
			     *src_bucket);

	count_new_entries  = 0;
	end_ptr            = &head;

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			/* done, add results list, expanding if necessary */
			if (src_bucket == last_src_bucket)
				return rbhm_add_list(dst_map,
						     dst_buckets,
						     dst_count_buckets_m1,
						     dst_factory_ptr,
						     head,
						     end_ptr,
						     count_new_entries);

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch bucket */
		dst_bucket_root
		= dst_buckets[src_hkey->hash & dst_count_buckets_m1];


		/* if new entry, append to list */
		if (!red_black_hfind(dst_bucket_root,
				     src_hkey)) {
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *src_hkey;

			++count_new_entries;
		}
	}
}


int
red_black_hmap_put_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict *restrict dst_buckets;
	struct RedBlackHNode *restrict dst_bucket_root;
	struct RedBlackHNodeFactory *restrict dst_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict *restrict end_ptr;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	unsigned int count_new_entries;
	unsigned int dst_count_buckets_m1;
	int status;

	dst_buckets          = dst_map->buckets;
	dst_count_buckets_m1 = dst_map->count.buckets_m1;
	dst_factory_ptr      = &dst_map->factory;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0) {
		rbhnf_destroy(dst_factory_ptr); /* destroy list */
		return -1; /* RED_BLACK_MALLOC ERROR */
	}

	factory_buffer_ptr = &dst_factory_ptr->buffer;

	src_bucket      = src_map->buckets;
	last_src_bucket = src_bucket + src_map->count.buckets_m1;

	red_black_hitor_init(&src_bucket_itor,
			     *src_bucket);

	count_new_entries  = 0;
	end_ptr            = &head;

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			/* done, add results list, expanding if necessary */
			if (src_bucket == last_src_bucket)
				return rbhm_add_list(dst_map,
						     dst_buckets,
						     dst_count_buckets_m1,
						     dst_factory_ptr,
						     head,
						     end_ptr,
						     count_new_entries);

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch bucket */
		dst_bucket_root
		= dst_buckets[src_hkey->hash & dst_count_buckets_m1];


		/* if new entry, append to list, else replace old key */
		if (!red_black_hreplace(dst_bucket_root,
					src_hkey)) {
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *src_hkey;

			++count_new_entries;
		}
	}
}


bool
red_black_hmap_add_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict buffer;
	struct RedBlackHNode *volatile restrict cursor;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *volatile restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict src_buckets;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNode *restrict dst_node;
	struct RedBlackHNode *restrict *restrict dst_buckets;
	unsigned int src_count_entries;
	unsigned int dst_count_entries;
	unsigned int dst_count_buckets_m1;
	unsigned int dst_count_max_capacity;
	unsigned int dst_count_buckets;
	unsigned int old_dst_count_buckets_m1;

	src_count_entries = src_map->count.entries;

	if (src_count_entries == 0)
		return true; /* do nothing */

	/* allocate nodes for new entries */
	buffer = rbhnf_allocate_nodes(&dst_map->factory,
				      src_count_entries);

	if (buffer == NULL)
		return false;

	dst_buckets            = dst_map->buckets;
	dst_count_buckets_m1   = dst_map->count.buckets_m1;
	dst_count_entries      = dst_map->count.entries + src_count_entries;
	dst_count_max_capacity = dst_map->count.max_capacity;

	if (dst_count_entries > dst_count_max_capacity) {
		/* need to expand */
		dst_count_max_capacity = NEXT_POWER_OF_TWO(dst_count_entries);
		dst_count_buckets      = dst_count_max_capacity
				       / RBHM_MAX_AVG_COLLISIONS;

		dst_buckets
		= RED_BLACK_REALLOC((void *) dst_buckets,
				    sizeof(*dst_buckets) * dst_count_buckets);

		if (dst_buckets == NULL)
			return false;

		/* expand */
		old_dst_count_buckets_m1 = dst_count_buckets_m1;
		dst_count_buckets_m1     = dst_count_buckets - 1;

		rbhm_reset_buckets(dst_buckets,
				   old_dst_count_buckets_m1,
				   dst_count_buckets_m1);

		/* update rest of map */
		dst_map->buckets	     = dst_buckets;
		dst_map->count.buckets_m1    = dst_count_buckets_m1;
		dst_map->count.max_capacity  = dst_count_max_capacity;
		dst_map->expand_factor      += RBHM_EXPAND_FACTOR_INC;

	}

	dst_map->count.entries = dst_count_entries;

	cursor = buffer;

	src_buckets     = src_map->buckets;
	last_src_bucket = src_buckets + src_map->count.buckets_m1;
	src_bucket      = src_buckets;

	red_black_hitor_init(&src_bucket_itor,
			     *src_buckets);

	/* dump src nodes into dst_map */
	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			if (src_bucket == last_src_bucket)
				return true; /* added all hkeys */

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch destination bucket */
		dst_bucket
		= &dst_buckets[src_hkey->hash & dst_count_buckets_m1];

		dst_node = cursor++; /* pop node from buffer */

		dst_node->hkey = *src_hkey; /* set hkey */

		/* add node to bucket */
		red_black_hadd(dst_bucket,
			       jump_buffer,
			       dst_node);
	}
}


int
red_black_hmap_delete_all(RedBlackHMap *const restrict dst_map,
			  const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict *restrict dst_buckets;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNodeFactory *restrict dst_factory_ptr;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *volatile restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict src_buckets;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	const struct RedBlackHKey *restrict src_hkey;
	unsigned int dst_count_buckets_m1;
	volatile unsigned int count;
	int status;
	unsigned int count_deleted;

	dst_buckets	     = dst_map->buckets;
	dst_count_buckets_m1 = dst_map->count.buckets_m1;
	dst_factory_ptr      = &dst_map->factory;

	src_buckets     = src_map->buckets;
	last_src_bucket = src_buckets + src_map->count.buckets_m1;
	src_bucket      = src_buckets;

	red_black_hitor_init(&src_bucket_itor,
			     *src_buckets);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* add 1 if jumped and deletion was made */
	count += RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			if (src_bucket == last_src_bucket) {
				/* traversed all hkeys, update map count and
				 * report total deleted */
				count_deleted = count;

				dst_map->count.entries -= count_deleted;

				return (int) count_deleted;
			}

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch destination bucket */
		dst_bucket
		= &dst_buckets[src_hkey->hash & dst_count_buckets_m1];

		/* delete hkey from bucket */
		count += red_black_hdelete(dst_bucket,
					   dst_factory_ptr,
					   jump_buffer,
					   src_hkey);
	}
}


void
red_black_hmap_drop_all(RedBlackHMap *const restrict dst_map,
			const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict *restrict dst_buckets;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNodeFactory *restrict dst_factory_ptr;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *volatile restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict src_buckets;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	const struct RedBlackHKey *restrict src_hkey;
	unsigned int dst_count_buckets_m1;

	src_buckets     = src_map->buckets;
	last_src_bucket = src_buckets + src_map->count.buckets_m1;
	src_bucket      = src_buckets;

	red_black_hitor_init(&src_bucket_itor,
			     *src_buckets);

	dst_buckets		= dst_map->buckets;
	dst_count_buckets_m1	= dst_map->count.buckets_m1;
	dst_map->count.entries -= src_map->count.entries;
	dst_factory_ptr		= &dst_map->factory;


	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			if (src_bucket == last_src_bucket)
				return; /* dropped all hkeys */

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch destination bucket */
		dst_bucket
		= &dst_buckets[src_hkey->hash & dst_count_buckets_m1];

		/* drop node from bucket */
		red_black_hdrop(dst_bucket,
				dst_factory_ptr,
				jump_buffer,
				src_hkey);
	}
}


/* build a map from a list of nodes */
static inline int
rbhm_from_list(RedBlackHMap *const restrict map,
	       struct RedBlackHNode *volatile restrict head,
	       const unsigned int count_entries,
	       struct RedBlackHNodeFactory *const restrict map_factory_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict *restrict bucket;
	RedBlackHash hash;
	unsigned int count_buckets;
	unsigned int count_max_capacity;

	if (count_entries < RBHM_INIT_MAX_CAPACITY) {
		count_max_capacity = RBHM_INIT_MAX_CAPACITY;
		count_buckets      = RBHM_INIT_BUCKET_COUNT;

	} else {
		/* 1. set 'count_max_capacity' to 'count_entries' rounded up to
		 *    the next power of two
		 * 2. determine 'count_buckets' by dividing 'count_max_capacity'
		 *    by 'RBHM_MAX_AVG_COLLISIONS' */
		count_max_capacity = NEXT_POWER_OF_TWO(count_entries);
		count_buckets      = count_max_capacity
				   / RBHM_MAX_AVG_COLLISIONS;
	}

	struct RedBlackHNode *restrict *const restrict buckets
	= RED_BLACK_CALLOC(count_buckets,
			   sizeof(*buckets));

	if (buckets == NULL) {
		rbhnf_destroy(map_factory_ptr); /* destroy list */
		return -1; /* RED_BLACK_MALLOC ERROR */
	}

	const unsigned int count_buckets_m1 = count_buckets - 1;

	/* dump list nodes into map */
	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		node = head;

		if (node == NULL)
			break;

		hash = node->hkey.hash;

		head = node->left;

		bucket = &buckets[hash & count_buckets_m1];

		red_black_hadd(bucket,
			       jump_buffer,
			       node);
	}

	/* initialize rest of map */
	map->buckets		= buckets;
	map->count.buckets_m1   = count_buckets_m1;
	map->count.entries      = count_entries;
	map->count.max_capacity = count_max_capacity;
	map->expand_factor	= RBHM_INIT_EXPAND_FACTOR;

	return (int) count_entries;
}


#if 1
int
red_black_hmap_union(RedBlackHMap *const restrict union_map,
		     const RedBlackHMap *const map1,
		     const RedBlackHMap *const map2)
{

	const RedBlackHMap *restrict clone_map;
	const RedBlackHMap *restrict insert_map;

	if (map1 == map2)
		return red_black_hmap_clone(union_map,
					    map1)
		     ? (int) union_map->count.entries
		     : -1; /* RED_BLACK_MALLOC failure */

	/* ensure larger of the two maps gets cloned first */
	if (map1->count.entries < map2->count.entries) {
		clone_map  = map2;
		insert_map = map1;

	} else {
		clone_map  = map1;
		insert_map = map2;
	}

	return (   red_black_hmap_clone(union_map,
					clone_map)
		&& (red_black_hmap_insert_all(union_map,
					      insert_map) >= 0))
	     ? (int) union_map->count.entries
	     : -1; /* RED_BLACK_MALLOC failure */
}
#else
int
red_black_hmap_union(RedBlackHMap *const restrict union_map,
		     const RedBlackHMap *const map1,
		     const RedBlackHMap *const map2)
{
	RedBlackJumpBuffer jump_buffer;
	const RedBlackHMap *restrict copy_map;
	const RedBlackHMap *restrict walk_map;
	struct RedBlackHNodeFactory *restrict union_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackHItor bucket_itor;
	struct RedBlackHNode *restrict copy_bucket_root;
	struct RedBlackHNode *const restrict *restrict copy_bucket;
	struct RedBlackHNode *const restrict *restrict last_copy_bucket;
	struct RedBlackHNode *const restrict *restrict copy_buckets;
	struct RedBlackHNode *const restrict *restrict walk_bucket;
	struct RedBlackHNode *const restrict *restrict last_walk_bucket;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_copy_buckets_m1;
	unsigned int count_entries;

	if (map1 == map2)
		return red_black_hmap_clone(union_map,
					    map1)
		     ? (int) union_map->count.entries
		     : -1; /* RED_BLACK_MALLOC failure */


	union_factory_ptr = &union_map->factory;

	rbhnf_init(union_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbhnf_destroy(union_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &union_factory_ptr->buffer;

	/* copy entries from and search in larger map */
	if (map1->count.entries < map2->count.entries) {
		walk_map = map1;
		copy_map = map2;

	} else {
		walk_map = map2;
		copy_map = map1;
	}

	walk_bucket	 = walk_map->buckets;
	last_walk_bucket = walk_bucket + walk_map->count.buckets_m1;

	copy_buckets	      = copy_map->buckets;
	count_copy_buckets_m1 = copy_map->count.buckets_m1;
	last_copy_bucket      = copy_buckets + count_copy_buckets_m1;
	copy_bucket	      = copy_buckets;

	red_black_hitor_init(&bucket_itor,
			     *copy_bucket);

	end_ptr       = &head; /* first entry will set 'head' */
	count_entries = 0;

	/* make list of all entries from 'copy_map' */
	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor);

			if (hkey != NULL)
				break;

			if (copy_bucket == last_copy_bucket)
				goto TRAVERSE_WALK_MAP; /* done copying */

			++copy_bucket;

			red_black_hitor_reset(&bucket_itor,
					      *copy_bucket);
		}

		/* copy all entres from copy_map */
		node = rbhnfb_allocate(factory_buffer_ptr,
				       jump_buffer);

		*end_ptr   = node;
		end_ptr    = &node->left;
		node->hkey = *hkey;

		++count_entries;
	}

	/* append unique entries of 'walk_map' */
	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor);

			if (hkey != NULL)
				break;

			if (walk_bucket == last_walk_bucket) {
				/* terminate list and dump into union_map */
				*end_ptr = NULL;

				return rbhm_from_list(union_map,
						      head,
						      count_entries,
						      union_factory_ptr);
			}

			++walk_bucket;

TRAVERSE_WALK_MAP:
			red_black_hitor_reset(&bucket_itor,
					      *walk_bucket);
		}

		copy_bucket_root
		= copy_buckets[hkey->hash & count_copy_buckets_m1];

		if (!red_black_hfind(copy_bucket_root,
				     hkey)) {
			/* unique entry found, append node to list */
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *hkey;

			++count_entries;
		}
	}

}
#endif


int
red_black_hmap_intersection(RedBlackHMap *const restrict intersection_map,
			    const RedBlackHMap *const map1,
			    const RedBlackHMap *const map2)
{
	const RedBlackHMap *restrict walk_map;
	const RedBlackHMap *restrict find_map;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHItor walk_bucket_itor;
	struct RedBlackHNode *const restrict *restrict walk_bucket;
	struct RedBlackHNode *const restrict *restrict last_walk_bucket;
	struct RedBlackHNode *const restrict *restrict find_buckets;
	struct RedBlackHNode *restrict find_bucket_root;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHNodeFactory *restrict intersection_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_find_buckets_m1;
	unsigned int count_entries;

	if (map1 == map2)
		return red_black_hmap_clone(intersection_map,
					    map1)
		     ? (int) intersection_map->count.entries
		     : -1; /* RED_BLACK_MALLOC failure */

	/* init intersection factory
	 * nodes will be allocated from here, joined into a list, and finally
	 * dumped all at once into intersection_map to avoid intermediate
	 * expansions */
	intersection_factory_ptr = &intersection_map->factory;

	rbhnf_init(intersection_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbhnf_destroy(intersection_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &intersection_factory_ptr->buffer;

	/* ensure buckets being traversed have fewer entries */
	if (map1->count.entries < map2->count.entries) {
		walk_map = map1;
		find_map = map2;

	} else {
		walk_map = map2;
		find_map = map1;
	}

	find_buckets	      = find_map->buckets;
	count_find_buckets_m1 = find_map->count.buckets_m1;

	walk_bucket	 = walk_map->buckets;
	last_walk_bucket = walk_bucket + walk_map->count.buckets_m1;

	red_black_hitor_init(&walk_bucket_itor,
			     *walk_bucket);

	end_ptr       = &head; /* first entry will set 'head' */
	count_entries = 0;

	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&walk_bucket_itor);

			if (hkey != NULL)
				break;

			if (walk_bucket == last_walk_bucket) {
				/* done, terminate list and dump into
				 * intersection_map */
				*end_ptr = NULL;

				return rbhm_from_list(intersection_map,
						      head,
						      count_entries,
						      intersection_factory_ptr);
			}

			++walk_bucket;

			red_black_hitor_reset(&walk_bucket_itor,
					      *walk_bucket);
		}

		/* fetch bucket */
		find_bucket_root
		= find_buckets[hkey->hash & count_find_buckets_m1];

		if (red_black_hfind(find_bucket_root,
				    hkey)) {
			/* intersection found, append node to list */
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *hkey;

			++count_entries;
		}
	}
}


int
red_black_hmap_difference(RedBlackHMap *const restrict difference_map,
			  const RedBlackHMap *const map1,
			  const RedBlackHMap *const map2)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHItor bucket_itor1;
	struct RedBlackHNode *const restrict *restrict bucket1;
	struct RedBlackHNode *const restrict *restrict last_bucket1;
	struct RedBlackHNode *const restrict *restrict buckets2;
	struct RedBlackHNode *restrict bucket2_root;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHNodeFactory *restrict difference_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_buckets2_m1;
	unsigned int count_entries;

	if (map1 == map2) /* 0 if success, -1 if failure */
		return -((int) !red_black_hmap_init(difference_map));

	/* init difference factory
	 * nodes will be allocated from here, joined into a list, and finally
	 * dumped all at once into difference_map to avoid intermediate
	 * expansions */
	difference_factory_ptr = &difference_map->factory;

	rbhnf_init(difference_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbhnf_destroy(difference_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &difference_factory_ptr->buffer;

	buckets2	  = map2->buckets;
	count_buckets2_m1 = map2->count.buckets_m1;

	bucket1      = map1->buckets;
	last_bucket1 = bucket1 + map1->count.buckets_m1;

	red_black_hitor_init(&bucket_itor1,
			     *bucket1);

	end_ptr       = &head; /* first entry will set 'head' */
	count_entries = 0;

	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor1);

			if (hkey != NULL)
				break;

			if (bucket1 == last_bucket1) {
				/* done, terminate list and dump into
				 * difference_map */
				*end_ptr = NULL;

				return rbhm_from_list(difference_map,
						      head,
						      count_entries,
						      difference_factory_ptr);
			}

			++bucket1;

			red_black_hitor_reset(&bucket_itor1,
					      *bucket1);
		}

		/* fetch bucket */
		bucket2_root = buckets2[hkey->hash & count_buckets2_m1];

		if (!red_black_hfind(bucket2_root,
				     hkey)) {
			/* hkey unique to map1 found, append node to list */
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *hkey;

			++count_entries;
		}
	}
}


int
red_black_hmap_sym_difference(RedBlackHMap *const restrict sym_difference_map,
			      const RedBlackHMap *const map1,
			      const RedBlackHMap *const map2)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHItor bucket_itor;
	struct RedBlackHNode *const restrict *restrict bucket1;
	struct RedBlackHNode *const restrict *restrict last_bucket1;
	struct RedBlackHNode *const restrict *restrict bucket2;
	struct RedBlackHNode *const restrict *restrict last_bucket2;
	struct RedBlackHNode *const restrict *restrict buckets1;
	struct RedBlackHNode *const restrict *restrict buckets2;
	struct RedBlackHNode *restrict bucket_root;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHNodeFactory *restrict sym_diff_factory_ptr;
	struct RedBlackHNodeFactoryBuffer *restrict factory_buffer_ptr;
	const struct RedBlackHKey *restrict hkey;
	unsigned int count_buckets1_m1;
	unsigned int count_buckets2_m1;
	unsigned int count_entries;

	if (map1 == map2) /* 0 if success, -1 if failure */
		return  -((int) !red_black_hmap_init(sym_difference_map));

	/* init sym_difference factory
	 * nodes will be allocated from here, joined into a list, and finally
	 * dumped all at once into difference_map to avoid intermediate
	 * expansions */

	sym_diff_factory_ptr = &sym_difference_map->factory;

	rbhnf_init(sym_diff_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbhnf_destroy(sym_diff_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &sym_diff_factory_ptr->buffer;

	buckets2	  = map2->buckets;
	count_buckets2_m1 = map2->count.buckets_m1;
	last_bucket2      = buckets2 + count_buckets2_m1;
	bucket2		  = buckets2;

	buckets1	  = map1->buckets;
	count_buckets1_m1 = map1->count.buckets_m1;
	last_bucket1	  = buckets1 + count_buckets1_m1;
	bucket1		  = buckets1;

	/* traverse map1 */
	red_black_hitor_init(&bucket_itor,
			     *bucket1);

	end_ptr       = &head; /* first entry will set 'head' */
	count_entries = 0;

	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor);

			if (hkey != NULL)
				break;

			if (bucket1 == last_bucket1) /* done traversing map1 */
				goto TRAVERSE_MAP2;

			++bucket1;

			red_black_hitor_reset(&bucket_itor,
					      *bucket1);
		}

		/* fetch bucket */
		bucket_root = buckets2[hkey->hash & count_buckets2_m1];

		if (!red_black_hfind(bucket_root,
				     hkey)) {
			/* hkey unique to map1 found, append node to list */
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *hkey;

			++count_entries;
		}
	}

	/* traverse map2 */
	while (1) {
		/* fetch next hkey */
		while (1) {
			hkey = red_black_hitor_next_hkey(&bucket_itor);

			if (hkey != NULL)
				break;

			if (bucket2 == last_bucket2) {
				/* done, terminate list and dump into
				 * sym_difference_map */
				*end_ptr = NULL;

				return rbhm_from_list(sym_difference_map,
						      head,
						      count_entries,
						      sym_diff_factory_ptr);
			}

			++bucket2;
TRAVERSE_MAP2:
			red_black_hitor_reset(&bucket_itor,
					      *bucket2);
		}

		/* fetch bucket */
		bucket_root = buckets1[hkey->hash & count_buckets1_m1];

		if (!red_black_hfind(bucket_root,
				     hkey)) {
			/* hkey unique to map2 found, append node to list */
			node = rbhnfb_allocate(factory_buffer_ptr,
					       jump_buffer);

			*end_ptr   = node;
			end_ptr    = &node->left;
			node->hkey = *hkey;

			++count_entries;
		}
	}
}


void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
			 const RedBlackHMap *const restrict map)
{
	struct RedBlackHNode *const restrict *restrict first_bucket;

	first_bucket = map->buckets;

	/* initialize first bucket itor */
	red_black_hitor_init(&itor->bucket_itor,
			     *first_bucket);

	itor->bucket      = first_bucket;
	itor->last_bucket = first_bucket + map->count.buckets_m1;
}


void
red_black_hmap_itor_reset(RedBlackHMapItor *const restrict itor,
			  const RedBlackHMap *const restrict map)
{
	struct RedBlackHNode *const restrict *restrict first_bucket;

	first_bucket = map->buckets;

	/* initialize first bucket itor */
	red_black_hitor_reset(&itor->bucket_itor,
			      *first_bucket);

	itor->bucket      = first_bucket;
	itor->last_bucket = first_bucket + map->count.buckets_m1;
}


bool
red_black_hmap_itor_next(RedBlackHMapItor *const restrict itor,
			 void **const restrict key_ptr,
			 size_t *const restrict length_ptr)
{
	struct RedBlackHItor *restrict bucket_itor;
	struct RedBlackHNode *const restrict *restrict bucket;
	struct RedBlackHNode *const restrict *restrict last_bucket;

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
				      *bucket);

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
