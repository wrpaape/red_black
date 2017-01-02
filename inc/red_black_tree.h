#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"  /* Comparator */
#include "red_black_allocator.h"   /* Node, Allocator, JumpBuffer, bool */
#include "red_black_iterator.h"    /* RedBlackIterator */
#include "red_black_print_types.h" /* KeySizer|Putter, size_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct _RedBlackTree {
	struct RedBlackNode *root;
	RedBlackComparator comparator;
	struct RedBlackAllocator allocator;
};

typedef struct _RedBlackTree RedBlackTree;

typedef RedBlackIterator RedBlackTreeIterator;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator);
void
red_black_tree_destroy(RedBlackTree *const restrict tree);


int
red_black_tree_insert(RedBlackTree *const restrict tree,
		      const void *const key);
/* RETURNS
 *
 *  1	'key' was NOT found in 'tree',	'tree' has been updated
 *  0	'key' was found in tree,	'tree' was NOT updated
 * -1	OUT OF MEMORY,			'tree' was NOT updated
 * ────────────────────────────────────────────────────────────────────────── */

bool
red_black_tree_delete(RedBlackTree *const restrict tree,
		      const void *const key);

bool
red_black_tree_find(RedBlackTree *const restrict tree,
		    const void *const key);
bool
red_black_tree_verify(RedBlackTree *const restrict tree);

bool
red_black_tree_print(RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter);

unsigned int
red_black_tree_count(RedBlackTree *const restrict tree);

void
red_black_tree_iterator_init_asc(RedBlackTreeIterator *const restrict iterator,
				 RedBlackTree *const restrict tree);

void
red_black_tree_iterator_init_desc(RedBlackTreeIterator *const restrict iterator,
				 RedBlackTree *const restrict tree);

bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iterator,
			     const void **const restrict key_ptr);


#endif /* ifndef RED_BLACK_TREE_H_ */
