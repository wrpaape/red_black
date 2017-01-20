#ifndef RED_BLACK_DELETE_H_
#define RED_BLACK_DELETE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key);

int
red_black_delete_min(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer);

int
red_black_delete_max(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no deletion made, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful deletion, tree updated
 *	0				OK, no deletion made, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_DELETE_H_ */
