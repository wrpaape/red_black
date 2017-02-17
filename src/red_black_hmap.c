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
#include <limits.h>				 /* CHAR_BIT */





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
rbhm_reset_buckets(struct RedBlackHNode *restrict *const restrict buckets,
		   const unsigned int old_count,
		   const unsigned int new_count_m1)
{
	RedBlackHash hash;
	struct RedBlackHNode *restrict node;
	struct RedBlackHNode *volatile restrict next;
	struct RedBlackHNode *restrict head;
	struct RedBlackHNode *restrict *restrict end_ptr;
	struct RedBlackHNode *restrict *restrict bucket;
	struct RedBlackHNode *restrict *restrict first_new_bucket;
	struct RedBlackHNode *restrict *restrict last_new_bucket;
	RedBlackJumpBuffer jump_buffer;

	first_new_bucket = buckets + old_count;
	last_new_bucket  = buckets + new_count_m1;
	bucket           = buckets;

	end_ptr = &head; /* concat 1st list with itself, set head */

	/* traverse old buckets in old portion */
	do {
		node    = *bucket; /* remove tree */
		*bucket = NULL;

		/* gather tree nodes into list (node->left->...->NULL) */
		end_ptr = red_black_hconcat(node,
					    end_ptr);

		++bucket;
	} while (bucket < first_new_bucket);

	/* initialize newly allocated portion */
	do {
		*bucket++ = NULL;
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
		red_black_hadd(bucket,
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

	/* reallocate buckets buffer */
	struct RedBlackHNode *restrict *const restrict new_buckets
	= RED_BLACK_REALLOC((void *) map->buckets,
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
	struct RedBlackHNode *restrict *const restrict new_buckets	\
	= RED_BLACK_REALLOC((void *) MAP->buckets,			\
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
	struct RedBlackHNode *restrict *restrict bucket;
	struct RedBlackHNode *restrict *restrict bucket_until;
	bool status;

	bucket = RED_BLACK_MALLOC(  sizeof(*bucket)
				  * RBHM_INIT_BUCKET_COUNT);

	status = (bucket != NULL);

	if (status) {
		map->buckets		= bucket;
		map->count.buckets_m1   = RBHM_INIT_BUCKET_COUNT - 1;
		map->count.entries      = 0;
		map->count.max_capacity = RBHM_INIT_BUCKET_COUNT
					* RBHM_MAX_AVG_COLLISIONS;
		map->expand_factor	= RBHM_INIT_EXPAND_FACTOR;

		rbhnf_init(&map->factory);

		bucket_until = bucket + RBHM_INIT_BUCKET_COUNT;

		/* initialize buckets */
		do {
			*bucket++ = NULL;
		} while (bucket < bucket_until);

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

	dst_bucket = RED_BLACK_MALLOC(  sizeof(*dst_bucket)
				      * (count_buckets_m1 + 1));

	if (dst_bucket == NULL)
		return false;

	count_entries = src_map->count.entries;

	dst_map->buckets	    = dst_bucket;
	dst_map->count.buckets_m1   = count_buckets_m1;
	dst_map->count.entries      = count_entries;
	dst_map->count.max_capacity = src_map->count.max_capacity;
	dst_map->expand_factor      = src_map->expand_factor;

	last_dst_bucket = dst_bucket + count_buckets_m1;

	if (count_entries == 0) {
		/* finish empty initialization */
		rbhnf_init(&dst_map->factory);

		do {
			*dst_bucket++ = NULL;
		} while (dst_bucket <= last_dst_bucket);

	} else {
		/* copy src_map buckets,
		 * allocate nodes from first bucket factory */
		buffer = rbhnf_init_w_nodes(&dst_map->factory,
					    count_entries);

		if (buffer == NULL) {
			RED_BLACK_FREE((void *) dst_bucket);
			return false;
		}

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
		status = red_black_hupdate(bucket,
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

	return true;
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
rb_hmap_congruent(const RedBlackHMap *const restrict map1,
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

		status = (count_buckets_m1     == map2->count.buckets_m1)
		      && (map1->count.entries  == map2->count.entries)
		      && rb_hmap_congruent(map1,
					   map2,
					   count_buckets_m1);
	}

	return status;
}


static inline bool
rb_hmap_similar_walk(struct RedBlackHNode *const restrict *restrict bucket1,
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
		while (1) {
			hkey1 = red_black_hitor_next_hkey(&bucket_itor1);

			if (hkey1 != NULL)
				break;

			if (bucket1 == last_bucket1)
				return true; /* all hkeys equal */

			++bucket1;

			red_black_hitor_reset(&bucket_itor1,
					      *bucket1);
		}


		while (1) {
			hkey2 = red_black_hitor_next_hkey(&bucket_itor2);

			if (hkey2 != NULL)
				break;

			++bucket2; /* no need to check for last */

			red_black_hitor_reset(&bucket_itor2,
					      *bucket2);
		}

		if (red_black_hkey_comparator(hkey1,
					      hkey2) != 0)
			return false;
	}
}

static inline bool
rb_hmap_similar_find(struct RedBlackHNode *const restrict *restrict buckets1,
		     struct RedBlackHNode *const restrict *restrict buckets2,
		     const unsigned int count_buckets1_m1,
		     const unsigned int count_buckets2_m1)
{
	struct RedBlackHItor walk_bucket_itor;
	struct RedBlackHNode *const restrict *restrict walk_bucket;
	struct RedBlackHNode *const restrict *restrict last_walk_bucket;
	struct RedBlackHNode *const restrict *restrict find_buckets;
	const struct RedBlackHNode *restrict find_bucket_root;
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
			       ? rb_hmap_similar_walk(buckets1,
						      buckets2,
						      count_buckets1_m1)
			       : rb_hmap_similar_find(buckets1,
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
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNode *const restrict *restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	int status;


	src_bucket      = src_map->buckets;
	last_src_bucket = src_bucket + src_map->count.buckets_m1;

	red_black_hitor_init(&src_bucket_itor,
			     *src_bucket);

	const unsigned int init_dst_count_entries = dst_map->count.entries;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE) {
		++(dst_map->count.entries);
		goto CHECK_EXPAND;

	} else if (status < 0) {
		return -1;
	}

	/* first entry or attempted to insert duplicate (no insertion) */

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
					      *src_bucket);
		}

		/* fetch bucket */
		dst_bucket
		= &dst_map->buckets[src_hkey->hash & dst_map->count.buckets_m1];

		status = red_black_hinsert(dst_bucket,
					   &dst_map->factory,
					   jump_buffer,
					   src_hkey); /* 1, 0 */

		dst_map->count.entries += status;
CHECK_EXPAND:
		if (dst_map->count.entries > dst_map->count.max_capacity)
			RBHM_EXPAND(dst_map);
	}
}


int
red_black_hmap_put_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map)
{
	RedBlackJumpBuffer jump_buffer;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNode *const restrict *restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	int status;


	src_bucket      = src_map->buckets;
	last_src_bucket = src_bucket + src_map->count.buckets_m1;

	red_black_hitor_init(&src_bucket_itor,
			     *src_bucket);

	const unsigned int init_dst_count_entries = dst_map->count.entries;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE) {
		++(dst_map->count.entries);
		goto CHECK_EXPAND;

	} else if (status < 0) {
		return -1;
	}

	/* first entry or attempted to insert duplicate (no insertion) */

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
					      *src_bucket);
		}

		/* fetch bucket */
		dst_bucket
		= &dst_map->buckets[src_hkey->hash & dst_map->count.buckets_m1];

		status = red_black_hput(dst_bucket,
					&dst_map->factory,
					jump_buffer,
					src_hkey); /* 1, 0 */

		dst_map->count.entries += status;
CHECK_EXPAND:
		if (dst_map->count.entries > dst_map->count.max_capacity)
			RBHM_EXPAND(dst_map);
	}
}


static inline void
rbhm_add_all_dump(struct RedBlackHNode *restrict *const restrict dst_buckets,
		  const unsigned int dst_cnt_buckets_m1,
		  const RedBlackHMap *const restrict src_map,
		  struct RedBlackHNode *volatile restrict buffer)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackHItor src_bucket_itor;
	struct RedBlackHNode *const restrict *volatile restrict src_bucket;
	struct RedBlackHNode *const restrict *restrict src_buckets;
	struct RedBlackHNode *const restrict *restrict last_src_bucket;
	const struct RedBlackHKey *restrict src_hkey;
	struct RedBlackHNode *restrict *restrict dst_bucket;
	struct RedBlackHNode *restrict dst_node;

	src_buckets     = src_map->buckets;
	last_src_bucket = src_buckets + src_map->count.buckets_m1;
	src_bucket      = src_buckets;

	red_black_hitor_init(&src_bucket_itor,
			     *src_buckets);

	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (1) {
		/* fetch next hkey */
		while (1) {
			src_hkey = red_black_hitor_next_hkey(&src_bucket_itor);

			if (src_hkey != NULL)
				break;

			if (src_bucket == last_src_bucket)
				return; /* added all hkeys */

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch destination bucket */
		dst_bucket = &dst_buckets[src_hkey->hash & dst_cnt_buckets_m1];

		dst_node = buffer++; /* pop node from buffer */

		dst_node->hkey = *src_hkey; /* set hkey */

		/* add node to bucket */
		red_black_hadd(dst_bucket,
			       jump_buffer,
			       dst_node);
	}
}

bool
red_black_hmap_add_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map)
{
	struct RedBlackHNode *restrict buffer;
	struct RedBlackHNode *restrict *restrict buckets;
	unsigned int src_cnt_entries;
	unsigned int new_cnt_entries;
	unsigned int old_cnt_max_cap;
	unsigned int new_cnt_max_cap;
	unsigned int cnt_buckets_m1;
	unsigned int old_cnt_buckets;
	unsigned int new_cnt_buckets;
	int ctz_new_cnt_max_cap;
	int shift_expand;

	src_cnt_entries = src_map->count.entries;

	if (src_cnt_entries == 0)
		return true; /* do nothing */


	/* allocate nodes for new entries */
	buffer = rbhnf_allocate_nodes(&dst_map->factory,
				      src_cnt_entries);

	if (buffer == NULL)
		return false;

	buckets         = dst_map->buckets;
	cnt_buckets_m1  = dst_map->count.buckets_m1;
	new_cnt_entries = dst_map->count.entries + src_cnt_entries;
	old_cnt_max_cap = dst_map->count.max_capacity;

	/* check if need to expand */
	if (new_cnt_entries > old_cnt_max_cap) {
		/* set 'new_cnt_max_cap' to next power of two of
		 * 'new_cnt_entries' -- note that max capacity is always a
		 * power of two */

		/* get count of TRAILING zeros (ctz/power of two) of most
		 * significant bit of 'new_cnt_entries' */
		ctz_new_cnt_max_cap = (CHAR_BIT * sizeof(unsigned int))
				    - __builtin_clz(new_cnt_entries);

		/* shift by power to determine new max capacity */
		new_cnt_max_cap = 1 << ctz_new_cnt_max_cap;

		/* get difference in shifts required for expansion */
		shift_expand = ctz_new_cnt_max_cap
			     - __builtin_ctz(old_cnt_max_cap);

		/* apply shift to bucket count */
		old_cnt_buckets = cnt_buckets_m1 + 1;

		new_cnt_buckets = old_cnt_buckets << shift_expand;

		/* reallocate buckets */
		buckets = RED_BLACK_REALLOC((void *) buckets,
					    sizeof(*buckets) * new_cnt_buckets);

		if (buckets == NULL) {
			RED_BLACK_FREE((void *) buffer); /* free earlier allocation */
			return false;
		}

		cnt_buckets_m1 = new_cnt_buckets - 1;

		/* update dst_map fields -- apply 1 increment to expand
		 * factor, regardless of the size of expansion */
		dst_map->buckets	     = buckets;
		dst_map->count.buckets_m1    = cnt_buckets_m1;
		dst_map->count.max_capacity  = new_cnt_max_cap;
		dst_map->expand_factor      += RBHM_EXPAND_FACTOR_INC;

		/* scatter old entries into new buckets */
		rbhm_reset_buckets(buckets,
				   old_cnt_buckets,
				   cnt_buckets_m1);
	}

	dst_map->count.entries = new_cnt_entries;

	/* add all the new entries */
	rbhm_add_all_dump(buckets,
			  cnt_buckets_m1,
			  src_map,
			  buffer);
	return true;
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
	unsigned int dst_cnt_buckets_m1;
	volatile int count;
	int status;
	int count_deleted;

	dst_buckets	   = dst_map->buckets;
	dst_cnt_buckets_m1 = dst_map->count.buckets_m1;
	dst_factory_ptr    = &dst_map->factory;

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

				return count_deleted;
			}

			++src_bucket;

			red_black_hitor_reset(&src_bucket_itor,
					      *src_bucket);
		}

		/* fetch destination bucket */
		dst_bucket = &dst_buckets[src_hkey->hash & dst_cnt_buckets_m1];

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
	unsigned int dst_cnt_buckets_m1;

	dst_buckets	   = dst_map->buckets;
	dst_cnt_buckets_m1 = dst_map->count.buckets_m1;
	dst_factory_ptr    = &dst_map->factory;

	src_buckets     = src_map->buckets;
	last_src_bucket = src_buckets + src_map->count.buckets_m1;
	src_bucket      = src_buckets;

	red_black_hitor_init(&src_bucket_itor,
			     *src_buckets);

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
		dst_bucket = &dst_buckets[src_hkey->hash & dst_cnt_buckets_m1];

		/* drop node from bucket */
		red_black_hdrop(dst_bucket,
				dst_factory_ptr,
				jump_buffer,
				src_hkey);
	}
}


void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
			 RedBlackHMap *const restrict map)
{
	struct RedBlackHNode *restrict *restrict first_bucket;

	first_bucket = map->buckets;

	/* initialize first bucket itor */
	red_black_hitor_init(&itor->bucket_itor,
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
	struct RedBlackHNode *restrict *restrict bucket;
	struct RedBlackHNode *restrict *restrict last_bucket;

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

	/* itor->bucket = last_bucket; /1* ensure subsequent calls return false *1/ */
	return false; /* all buckets traversed, no keys left */
}
