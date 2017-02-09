#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_ADD_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_ADD_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* struct RedBlackNode */
#include "red_black_tree/red_black_comparator.h" /* Comparator */
#include "red_black_common/red_black_jump.h"	 /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_add(struct RedBlackNode *restrict *const restrict tree,
	      const RedBlackComparator comparator,
	      RedBlackJumpBuffer jump_buffer,
	      struct RedBlackNode *const restrict node);

/* may jump or return after successful insertion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_ADD_H_ */
