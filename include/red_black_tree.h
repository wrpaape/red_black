#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, JumpBuffer, bool */
#include "red_black_iterator.h"     /* RedBlackIterator */
#include "red_black_print_types.h"  /* KeySizer|Putter, size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackTree {
	struct RedBlackNode *restrict root;
	RedBlackComparator comparator;
	struct RedBlackNodeFactory node_factory;
};
typedef struct _RedBlackTree RedBlackTree;

typedef struct RedBlackIterator RedBlackTreeIterator;


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
red_black_tree_update(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict old_ptr);

int
red_black_tree_delete(RedBlackTree *const restrict tree,
		      const void *const key);

int
red_black_tree_remove(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict key_ptr);

bool
red_black_tree_find(const RedBlackTree *const restrict tree,
		    const void *const key);

bool
red_black_tree_fetch(const RedBlackTree *const restrict tree,
		     const void *const key,
		     void **const restrict fetch_ptr);

unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree);

void
red_black_tree_asc_iterator_init(RedBlackTreeIterator *const restrict iter,
				 const RedBlackTree *const restrict tree);
void
red_black_tree_asc_iterator_set(RedBlackTreeIterator *const restrict iter,
				const RedBlackTree *const restrict tree);

void
red_black_tree_desc_iterator_init(RedBlackTreeIterator *const restrict iter,
				  const RedBlackTree *const restrict tree);
void
red_black_tree_desc_iterator_set(RedBlackTreeIterator *const restrict iter,
				 const RedBlackTree *const restrict tree);

bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iter,
			     void **const restrict key_ptr);

bool
red_black_tree_print(const RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter);

bool
red_black_tree_verify(const RedBlackTree *const restrict tree);


#endif /* ifndef RED_BLACK_TREE_H_ */
