#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HVERIFY_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HVERIFY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h"  /* RedBlackHNode, bool */
#include "red_black_common/red_black_jump.h" /* RedBlackJumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hverify(const struct RedBlackHNode *const restrict root,
		  RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HVERIFY_H_ */
