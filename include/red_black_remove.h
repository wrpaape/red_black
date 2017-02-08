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
		 RedBlackJumpBuffer jump_buffer,
		 const void *const key,
		 void **const restrict remove_ptr);

int
red_black_remove_min(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer jump_buffer,
		     void **const restrict remove_ptr);

int
red_black_remove_max(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer jump_buffer,
		     void **const restrict remove_ptr);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful removal, remove_ptr set
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no removal made, remove_ptr unset
 *
 * or return
 *
 *	1				OK, successful removal, remove_ptr set
 * 	0				OK, no removal made, key_ptr unset
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_REMOVE_H_ */
