#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HSET_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HSET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h" /* HNode, HKey */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hset(struct RedBlackHNode *restrict node,
	       const struct RedBlackHKey *const restrict hkey);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HSET_H_ */
