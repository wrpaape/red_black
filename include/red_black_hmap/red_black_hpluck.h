#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPLUCK_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPLUCK_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hpluck(struct RedBlackHNode *restrict *const restrict tree,
		 struct RedBlackHNodeFactory *const restrict factory,
		 RedBlackJumpBuffer jump_buffer,
		 const struct RedBlackHKey *const restrict hkey,
		 void **const restrict pluck_ptr);

/* may jump or return after successful deletion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HPLUCK_H_ */
