#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"  /* Comparator */
#include "red_black_allocator.h"   /* Node, Allocator, JumpBuffer, bool */
#include "red_black_print_types.h" /* KeySizer|Putter, size_t */


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

bool
red_black_tree_delete(struct RedBlackTree *const restrict tree,
		      const void *const key);

bool
red_black_tree_find(struct RedBlackTree *const restrict tree,
		    const void *const key)
bool
red_black_tree_verify(struct RedBlackTree *const restrict tree);

bool
red_black_tree_print(struct RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter);

bool
red_black_tree_find(struct RedBlackTree *const restrict tree,
		    const void *const key)


void
red_black_tree_destroy(struct RedBlackTree *const restrict tree);

#endif /* ifndef RED_BLACK_TREE_H_ */
