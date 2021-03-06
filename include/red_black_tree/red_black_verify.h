#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_VERIFY_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_VERIFY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"	 /* RedBlackNode, bool */
#include "red_black_tree/red_black_comparator.h" /* RedBlackComparator */
#include "red_black_common/red_black_jump.h"     /* RedBlackJumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_verify(const struct RedBlackNode *const restrict root,
		 const RedBlackComparator comparator,
		 RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_VERIFY_H_ */
