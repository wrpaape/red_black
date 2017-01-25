#ifndef RED_BLACK_SUBTRACT_H_
#define RED_BLACK_SUBTRACT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_subtract(struct RedBlackNode *restrict *const restrict tree,
		   const RedBlackComparator comparator,
		   struct RedBlackNodeFactory *const restrict factory,
		   RedBlackJumpBuffer *const restrict jump_buffer,
		   const void *const key);

void
red_black_subtract_min(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNodeFactory *const restrict factory,
		       RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_subtract_max(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNodeFactory *const restrict factory,
		       RedBlackJumpBuffer *const restrict jump_buffer);

/* may jump or return after successful deletion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_SUBTRACT_H_ */
