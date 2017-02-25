#ifndef RED_BLACK_RED_BLACK_HMAP_H_
#define RED_BLACK_RED_BLACK_HMAP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNodeFactory, HNode */
#include "red_black_hmap/red_black_hitor.h"	    /* HItor */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHMapCount {
	unsigned int buckets_m1;
	unsigned int entries;
	unsigned int max_capacity;
};

struct _RedBlackHMap {
	struct RedBlackHNode *restrict *restrict buckets;
	struct RedBlackHMapCount count;
	unsigned int expand_factor;
	struct RedBlackHNodeFactory factory;
};
typedef struct _RedBlackHMap RedBlackHMap;

struct _RedBlackHMapItor {
	struct RedBlackHItor bucket_itor;
	struct RedBlackHNode *const restrict *restrict bucket;
	struct RedBlackHNode *const restrict *restrict last_bucket;
};
typedef struct _RedBlackHMapItor RedBlackHMapItor;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hmap_init(RedBlackHMap *const restrict map);

bool
red_black_hmap_clone(RedBlackHMap *const restrict dst_map,
		     const RedBlackHMap *const restrict src_map);

void
red_black_hmap_destroy(RedBlackHMap *const restrict map);

int
red_black_hmap_insert(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_put(RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length);

int
red_black_hmap_update_set(RedBlackHMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict old_ptr);

int
red_black_hmap_update_get(RedBlackHMap *const restrict map,
			  const void *const key,
			  const size_t length,
			  void **const restrict old_ptr);

bool
red_black_hmap_add(RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length);

int
red_black_hmap_delete(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length);

int
red_black_hmap_remove(RedBlackHMap *const restrict map,
		      const void *const key,
		      const size_t length,
		      void **const restrict remove_ptr);

void
red_black_hmap_drop(RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length);

void *
red_black_hmap_pluck(RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length);

bool
red_black_hmap_find(const RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length);

bool
red_black_hmap_fetch(const RedBlackHMap *const restrict map,
		     const void *const key,
		     const size_t length,
		     void **const restrict fetch_ptr);

bool
red_black_hmap_replace(const RedBlackHMap *const restrict map,
		       const void *const key,
		       const size_t length);

bool
red_black_hmap_exchange(const RedBlackHMap *const restrict map,
			const void *const key,
			const size_t length,
			void **const restrict old_ptr);

void *
red_black_hmap_get(const RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length);

void
red_black_hmap_set(const RedBlackHMap *const restrict map,
		   const void *const key,
		   const size_t length);

void *
red_black_hmap_swap(const RedBlackHMap *const restrict map,
		    const void *const key,
		    const size_t length);

bool
red_black_hmap_empty(const RedBlackHMap *const restrict map);

unsigned int
red_black_hmap_count(const RedBlackHMap *const restrict map);


bool
red_black_hmap_verify(const RedBlackHMap *const restrict map);

bool
red_black_hmap_congruent(const RedBlackHMap *const map1,
			 const RedBlackHMap *const map2);

bool
red_black_hmap_similar(const RedBlackHMap *const map1,
		       const RedBlackHMap *const map2);

bool
red_black_hmap_intersect(const RedBlackHMap *const map1,
			 const RedBlackHMap *const map2);

bool
red_black_hmap_subset(const RedBlackHMap *const map1,
		      const RedBlackHMap *const map2);

int
red_black_hmap_insert_all(RedBlackHMap *const restrict dst_map,
			  const RedBlackHMap *const restrict src_map);

int
red_black_hmap_put_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map);

bool
red_black_hmap_add_all(RedBlackHMap *const restrict dst_map,
		       const RedBlackHMap *const restrict src_map);

int
red_black_hmap_delete_all(RedBlackHMap *const restrict dst_map,
			  const RedBlackHMap *const restrict src_map);

void
red_black_hmap_drop_all(RedBlackHMap *const restrict dst_map,
			const RedBlackHMap *const restrict src_map);

int
red_black_hmap_union(RedBlackHMap *const restrict union_map,
		     const RedBlackHMap *const map1,
		     const RedBlackHMap *const map2);

int
red_black_hmap_intersection(RedBlackHMap *const restrict intersection_map,
			    const RedBlackHMap *const map1,
			    const RedBlackHMap *const map2);

int
red_black_hmap_difference(RedBlackHMap *const restrict difference_map,
			  const RedBlackHMap *const map1,
			  const RedBlackHMap *const map2);

int
red_black_hmap_sym_difference(RedBlackHMap *const restrict sym_difference_map,
			      const RedBlackHMap *const map1,
			      const RedBlackHMap *const map2);

void
red_black_hmap_itor_init(RedBlackHMapItor *const restrict itor,
			 const RedBlackHMap *const restrict map);

void
red_black_hmap_itor_reset(RedBlackHMapItor *const restrict itor,
			  const RedBlackHMap *const restrict map);

bool
red_black_hmap_itor_next(RedBlackHMapItor *const restrict itor,
			 void **const restrict key_ptr,
			 size_t *const restrict length_ptr);
#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_H_ */
