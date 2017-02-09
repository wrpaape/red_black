#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PUT_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_PUT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_comparator.h"   /* Comparator */
#include "red_black_tree/red_black_node_factory.h" /* Node, NodeFactory, JUMP */

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_put(struct RedBlackNode *restrict *const restrict tree,
	      const RedBlackComparator comparator,
	      struct RedBlackNodeFactory *const restrict factory,
	      RedBlackJumpBuffer jump_buffer,
	      const void *const key);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, successful key swap
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion
 *	0				OK, successful key swap
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PUT_H_ */
