#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HREMOVE_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HREMOVE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode_factory.h" /* HNode, Factory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
int
red_black_hremove(struct RedBlackHNode *restrict *const restrict tree,
		  struct RedBlackHNodeFactory *const restrict factory,
		  RedBlackJumpBuffer jump_buffer,
		  const struct RedBlackHKey *const restrict hkey,
		  void **const restrict remove_ptr);

/* may jump w/ a RED_BLACK_JUMP_VALUE_2 value of
 *
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful removal, remove_ptr set
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no removal made, remove_ptr unset
 *
 * or return
 *
 *	1				OK, successful removal, remove_ptr set
 * 	0				OK, no removal made, key_ptr unset
 * ────────────────────────────────────────────────────────────────────────── */
#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HREMOVE_H_ */
