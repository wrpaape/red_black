#ifndef RED_BLACK_HADD_H_
#define RED_BLACK_HADD_H_


/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hnode.h"	  /* struct RedBlackNode */
#include "red_black_jump.h"       /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_add(struct RedBlackHNode *restrict *const restrict tree,
	      RedBlackJumpBuffer jump_buffer,
	      struct RedBlackHNode *const restrict node);

/* may jump or return after successful insertion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_HADD_H_ */
