#ifndef RED_BLACK_INSERT_H_
#define RED_BLACK_INSERT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_insert(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no insertion made, tree NOT updated
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion, tree updated
 *	0				OK, no insertion made, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_INSERT_H_ */
