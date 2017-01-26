#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, JumpBuffer, bool */
#include "red_black_etor.h"         /* RedBlackEtor */
#include "red_black_print_types.h"  /* KeySizer|Putter, size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackTree {
	struct RedBlackNode *restrict root;
	RedBlackComparator comparator;
	struct RedBlackNodeFactory node_factory;
};
typedef struct _RedBlackTree RedBlackTree;

typedef struct RedBlackEtor RedBlackTreeEtor;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator);

bool
red_black_tree_clone(RedBlackTree *const restrict dst_tree,
		     const RedBlackTree *const restrict src_tree);
bool
rb_tree_clone(RedBlackTree *const restrict dst_tree,
	      const RedBlackTree *const restrict src_tree,
	      const unsigned int count);

void
red_black_tree_destroy(RedBlackTree *const restrict tree);

int
red_black_tree_insert(RedBlackTree *const restrict tree,
		      const void *const key);

int
red_black_tree_put(RedBlackTree *const restrict tree,
		   const void *const key);

int
red_black_tree_update(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict old_ptr);

bool
red_black_tree_add(RedBlackTree *const restrict tree,
		   const void *const key);

int
red_black_tree_delete(RedBlackTree *const restrict tree,
		      const void *const key);
int
red_black_tree_delete_min(RedBlackTree *const restrict tree);
int
red_black_tree_delete_max(RedBlackTree *const restrict tree);

int
red_black_tree_remove(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict remove_ptr);
int
red_black_tree_remove_min(RedBlackTree *const restrict tree,
			  void **const restrict remove_ptr);
int
red_black_tree_remove_max(RedBlackTree *const restrict tree,
			  void **const restrict remove_ptr);

void
red_black_tree_drop(RedBlackTree *const restrict tree,
		    const void *const key);
void
red_black_tree_drop_min(RedBlackTree *const restrict tree);
void
red_black_tree_drop_max(RedBlackTree *const restrict tree);

void *
red_black_tree_pluck(RedBlackTree *const restrict tree,
		     const void *const key);
void *
red_black_tree_pluck_min(RedBlackTree *const restrict tree);
void *
red_black_tree_pluck_max(RedBlackTree *const restrict tree);

bool
red_black_tree_find(const RedBlackTree *const restrict tree,
		    const void *const key);

bool
red_black_tree_fetch(const RedBlackTree *const restrict tree,
		     const void *const key,
		     void **const restrict fetch_ptr);
bool
red_black_tree_fetch_min(const RedBlackTree *const restrict tree,
			 void **const restrict fetch_ptr);
bool
red_black_tree_fetch_max(const RedBlackTree *const restrict tree,
			 void **const restrict fetch_ptr);

bool
red_black_tree_replace(const RedBlackTree *const restrict tree,
		       const void *const key);
bool
red_black_tree_replace_min(const RedBlackTree *const restrict tree,
			   const void *const key);
bool
red_black_tree_replace_max(const RedBlackTree *const restrict tree,
			   const void *const key);

bool
red_black_tree_exchange(const RedBlackTree *const restrict tree,
			const void *const key,
			void **const restrict old_ptr);
bool
red_black_tree_exchange_min(const RedBlackTree *const restrict tree,
			    const void *const key,
			    void **const restrict old_ptr);
bool
red_black_tree_exchange_max(const RedBlackTree *const restrict tree,
			    const void *const key,
			    void **const restrict old_ptr);

void *
red_black_tree_get(const RedBlackTree *const restrict tree,
		   const void *const key);
void *
red_black_tree_get_min(const RedBlackTree *const restrict tree);
void *
red_black_tree_get_max(const RedBlackTree *const restrict tree);


void
red_black_tree_set(const RedBlackTree *const restrict tree,
		   const void *const key);
void
red_black_tree_set_min(const RedBlackTree *const restrict tree,
		       const void *const key);
void
red_black_tree_set_max(const RedBlackTree *const restrict tree,
		       const void *const key);

void *
red_black_tree_swap(const RedBlackTree *const restrict tree,
		    const void *const key);
void *
red_black_tree_swap_min(const RedBlackTree *const restrict tree,
			const void *const key);
void *
red_black_tree_swap_max(const RedBlackTree *const restrict tree,
			const void *const key);

bool
red_black_tree_empty(const RedBlackTree *const restrict tree);

unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree);

bool
red_black_tree_verify(const RedBlackTree *const restrict tree);

bool
red_black_tree_congruent(const RedBlackTree *const tree1,
			 const RedBlackTree *const tree2);
bool
rb_tree_congruent(const RedBlackTree *const restrict tree1,
		  const RedBlackTree *const restrict tree2);

bool
red_black_tree_similar(const RedBlackTree *const tree1,
		       const RedBlackTree *const tree2);
bool
rb_tree_similar(const RedBlackTree *const restrict tree1,
		const RedBlackTree *const restrict tree2);

int
red_black_tree_insert_all(RedBlackTree *const restrict dst_tree,
			  const RedBlackTree *const restrict src_tree);

int
red_black_tree_put_all(RedBlackTree *const restrict dst_tree,
		       const RedBlackTree *const restrict src_tree);

bool
red_black_tree_add_all(RedBlackTree *const restrict dst_tree,
		       const RedBlackTree *const restrict src_tree);
bool
rb_tree_add_all(RedBlackTree *const restrict dst_tree,
		const RedBlackTree *const restrict src_tree,
		const unsigned int count_src);

int
red_black_tree_delete_all(RedBlackTree *const restrict dst_tree,
			  const RedBlackTree *const restrict src_tree);

void
red_black_tree_drop_all(RedBlackTree *const restrict dst_tree,
			const RedBlackTree *const restrict src_tree);

int
red_black_tree_union(RedBlackTree *const restrict union_tree,
		     const RedBlackTree *const restrict tree1,
		     const RedBlackTree *const restrict tree2);

int
rb_tree_union(RedBlackTree *const restrict union_tree,
	      const RedBlackTree *const restrict tree1,
	      const RedBlackTree *const restrict tree2,
	      const unsigned int count1);

int
red_black_tree_intersect(RedBlackTree *const restrict intersect_tree,
			 const RedBlackTree *const restrict tree1,
			 const RedBlackTree *const restrict tree2);

int
red_black_tree_except(RedBlackTree *const restrict except_tree,
		      const RedBlackTree *const restrict tree1,
		      const RedBlackTree *const restrict tree2);
int
rb_tree_except(RedBlackTree *const restrict except_tree,
	       const RedBlackTree *const restrict tree1,
	       const RedBlackTree *const restrict tree2,
	       const unsigned int count1);

bool
red_black_tree_print(const RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter);

void
red_black_tree_asc_etor_init(RedBlackTreeEtor *const restrict etor,
			     const RedBlackTree *const restrict tree);

void
red_black_tree_desc_etor_init(RedBlackTreeEtor *const restrict etor,
			      const RedBlackTree *const restrict tree);
void
red_black_tree_etor_reset(RedBlackTreeEtor *const restrict etor,
			  const RedBlackTree *const restrict tree);

bool
red_black_tree_etor_next(RedBlackTreeEtor *const restrict etor,
			 void **const restrict key_ptr);

#endif /* ifndef RED_BLACK_TREE_H_ */
