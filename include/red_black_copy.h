#ifndef RED_BLACK_COPY
#define RED_BLACK_COPY

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"  /* RedBlackNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNode *
red_black_copy_nodes(struct RedBlackNode *restrict *const restrict dst_tree,
		     const struct RedBlackNode *const restrict src_root,
		     struct RedBlackNode *restrict buffer);

struct RedBlackNode *
red_black_copy_hnodes(struct RedBlackNode *restrict *const restrict dst_tree,
		      const struct RedBlackNode *const restrict src_root,
		      struct RedBlackNode *restrict buffer);


#endif /* ifndef RED_BLACK_COPY */
