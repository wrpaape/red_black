#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HDELETE_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HDELETE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, HNodeFactory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hdelete(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no deletion made, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful deletion, tree updated
 *	0				OK, no deletion made, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HDELETE_H_ */
