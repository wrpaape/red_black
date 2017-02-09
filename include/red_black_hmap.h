#ifndef RED_BLACK_HMAP_H_
#define RED_BLACK_HMAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* RedBlackNodeFactory */
/* #include "red_black_hmap_count.h"   /1* RedBlackHMapCount *1/ */
#include "red_black_itor.h"	    /* RedBlackItor */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHBucket {
	struct RedBlackNode *restrict root;
	struct RedBlackNodeFactory node_factory;
};

struct RedBlackHMapCount {
	unsigned int buckets_m1;
	unsigned int entries;
	unsigned int max_capacity;
};

#define RBHM_VAR_EXPAND 1

struct _RedBlackHMap {
	struct RedBlackHBucket *restrict buckets;
	struct RedBlackHMapCount count;
#if RBHM_VAR_EXPAND
	unsigned int expand_factor;
#endif /* if RBHM_VAR_EXPAND */
};
typedef struct _RedBlackHMap RedBlackHMap;

struct _RedBlackHMapItor {
	struct RedBlackItor bucket_itor;
	struct RedBlackHBucket *restrict bucket;
	struct RedBlackHBucket *restrict last_bucket;
};
typedef struct _RedBlackHMapItor RedBlackHMapItor;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hmap_init(RedBlackHMap *const restrict map);

void
red_black_hmap_destroy(RedBlackHMap *const restrict map);

int
red_black_hmap_insert(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_update(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict old_ptr);

int
red_black_hmap_delete(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_remove(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict key_ptr);

bool
red_black_hmap_find(RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length);

bool
red_black_hmap_fetch(RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict key_ptr);

unsigned int
red_black_hmap_count(RedBlackHMap *const restrict map);


bool
red_black_hmap_verify(RedBlackHMap *const restrict map);

void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
			 RedBlackHMap *const restrict map);

bool
red_black_hmap_itor_next(RedBlackHMapItor *const restrict itor,
			 void **const restrict key_ptr,
			 size_t *const restrict length_ptr);
#endif /* ifndef RED_BLACK_HMAP_H_ */
