#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCOPY_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCOPY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h" /* RedBlackHNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHNode *
red_black_hcopy(struct RedBlackHNode *restrict *const restrict dst_tree,
		const struct RedBlackHNode *const restrict src_root,
		struct RedBlackHNode *restrict buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCOPY_H_ */
