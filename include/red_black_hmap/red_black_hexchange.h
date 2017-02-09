#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HEXCHANGE_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HEXCHANGE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h" /* HNode, bool */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hexchange(struct RedBlackHNode *restrict node,
		    const RedBlackComparator comparator,
		    const struct RedBlackHKey *const restrict hkey,
		    void **const restrict old_ptr);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HEXCHANGE_H_ */
