#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONGRUENT_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONGRUENT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h"  /* struct RedBlackHNode */
#include "red_black_common/red_black_jump.h" /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hcongruent(const struct RedBlackHNode *const restrict node1,
		     const struct RedBlackHNode *const restrict node2,
		     RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONGRUENT_H_ */
