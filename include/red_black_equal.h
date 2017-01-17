#ifndef RED_BLACK_EQUAL_H_
#define RED_BLACK_EQUAL_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* struct RedblackNode */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_jump.h"       /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_equal(const struct RedBlackNode *const restrict node1,
		const struct RedBlackNode *const restrict node2,
		const RedBlackComparator comparator,
		RedBlackJumpBuffer *const restrict jump_buffer);

#endif /* ifndef RED_BLACK_EQUAL_H_ */
