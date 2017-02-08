#ifndef RED_BLACK_VERIFY_H_
#define RED_BLACK_VERIFY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* RedBlackNode, bool */
#include "red_black_comparator.h" /* RedBlackComparator */
#include "red_black_jump.h"       /* RedBlackJumpBuffer */

bool
red_black_verify(const struct RedBlackNode *const restrict root,
		 const RedBlackComparator comparator,
		 RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_VERIFY_H_ */
