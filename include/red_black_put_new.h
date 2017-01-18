#ifndef RED_BLACK_PUT_NEW_H_
#define RED_BLACK_PUT_NEW_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	  /* struct RedblackNode */
#include "red_black_comparator.h" /* Comparator */
#include "red_black_jump.h"       /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_put_new(struct RedBlackNode *restrict *const restrict tree,
		  const RedBlackComparator comparator,
		  RedBlackJumpBuffer *const restrict jump_buffer,
		  struct RedBlackNode *const restrict node);

/* may jump or return after successful insertion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_PUT_NEW_H_ */
