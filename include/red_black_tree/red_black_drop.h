#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_DROP_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_DROP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_comparator.h"   /* Comparator */
#include "red_black_tree/red_black_node_factory.h" /* Node, NodeFactory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_drop(struct RedBlackNode *restrict *const restrict tree,
	       const RedBlackComparator comparator,
	       struct RedBlackNodeFactory *const restrict factory,
	       RedBlackJumpBuffer jump_buffer,
	       const void *const key);

void
red_black_drop_min(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNodeFactory *const restrict factory,
		   RedBlackJumpBuffer jump_buffer);

void
red_black_drop_max(struct RedBlackNode *restrict *const restrict tree,
		   struct RedBlackNodeFactory *const restrict factory,
		   RedBlackJumpBuffer jump_buffer);

/* may jump or return after successful deletion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_DROP_H_ */
