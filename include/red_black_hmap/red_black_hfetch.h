#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HFETCH_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HFETCH_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h" /* HNode, bool */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
bool
red_black_hfetch(const struct RedBlackHNode *restrict node,
		 const struct RedBlackHKey *const restrict hkey,
		 void **const restrict fetch_ptr);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HFETCH_H_ */
