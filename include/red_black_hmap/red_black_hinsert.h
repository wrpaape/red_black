#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HINSERT_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HINSERT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hinsert(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no insertion made, tree NOT updated
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion, tree updated
 *	0				OK, no insertion made, tree NOT updated
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HINSERT_H_ */
