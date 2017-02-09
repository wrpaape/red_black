#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPUT_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPUT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hput(struct RedBlackHNode *restrict *const restrict tree,
	       struct RedBlackHNodeFactory *const restrict factory,
	       RedBlackJumpBuffer jump_buffer,
	       const struct RedBlackHKey *const restrict hkey);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, successful key swap
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion
 *	0				OK, successful key swap
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPUT_H_ */
