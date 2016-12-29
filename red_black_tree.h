#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_allocator.h"  /* Node, Allocator, and JumpBuffer */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackTree {
	struct RedBlackNode *root;
	RedBlackComparator comparator;
	struct RedBlackAllocator allocator;
};

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_tree_init(struct RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator);

int
red_black_tree_insert(struct RedBlackTree *const restrict tree,
		      const void *const key);

/* RETURNS
 *
 *  1	'key' was NOT found in 'tree',	'tree' has been updated
 *  0	'key' was found in tree,	'tree' was NOT updated
 * -1	OUT OF MEMORY,			'tree' was NOT updated
 * ────────────────────────────────────────────────────────────────────────── */

void
red_black_tree_destroy(struct RedBlackTree *const restrict tree);

#endif /* ifndef RED_BLACK_TREE_H_ */
