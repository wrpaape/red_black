#ifndef RED_BLACK_CONGRUENT_H_
#define RED_BLACK_CONGRUENT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* struct RedblackNode */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_jump.h"       /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_congruent(const struct RedBlackNode *const restrict node1,
		    const struct RedBlackNode *const restrict node2,
		    const RedBlackComparator comparator,
		    RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_CONGRUENT_H_ */
