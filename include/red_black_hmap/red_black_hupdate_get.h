#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HUPDATE_GET_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HUPDATE_GET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hupdate_get(struct RedBlackHNode *restrict *const restrict tree,
		      struct RedBlackHNodeFactory *const restrict factory,
		      RedBlackJumpBuffer jump_buffer,
		      const struct RedBlackHKey *const restrict hkey,
		      void **const restrict old_ptr);

/* may jump w/ a RED_BLACK_JUMP_VALUE_3 value of
 *
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful insertion, old_ptr unset
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no key swap, old_ptr is set
 *	RED_BLACK_JUMP_VALUE_3_ERROR	OUT OF MEMORY, tree NOT updated
 *
 * or return
 *
 *	1				OK, successful insertion, old_ptr unset
 *	0				OK, no key swap, old_ptr is set
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HUPDATE_GET_H_ */
