#ifndef RED_BLACK_REMOVE_H_
#define RED_BLACK_REMOVE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_comparator.h"   /* Comparator */
#include "red_black_node_factory.h" /* Node, NodeFactory, and JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_remove(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key,
		 void **const restrict key_ptr);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful removal, key_ptr is set
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no removal made, key_ptr is unset
 *
 * or return
 *
 *	1				OK, successful removal, key_ptr is set
 * 	0				OK, no removal made, key_ptr is unset
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_REMOVE_H_ */
