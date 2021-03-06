#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_COPY_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_COPY_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h" /* RedBlackNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNode *
red_black_copy(struct RedBlackNode *restrict *const restrict dst_tree,
	       const struct RedBlackNode *const restrict src_root,
	       struct RedBlackNode *restrict buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_COPY_H_ */
