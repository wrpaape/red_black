#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HADD_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HADD_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h"  /* struct RedBlackHNode */
#include "red_black_common/red_black_jump.h" /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hadd(struct RedBlackHNode *restrict *const restrict tree,
	       RedBlackJumpBuffer jump_buffer,
	       struct RedBlackHNode *const restrict node);

/* may jump or return after successful insertion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HADD_H_ */
