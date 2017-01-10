#ifndef RED_BLACK_FLATTEN_H_
#define RED_BLACK_FLATTEN_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* struct RedBlackNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* turns a binary tree into a list (node->left->left-> ... left->NULL) */
void
red_black_flatten(struct RedBlackNode *const restrict root);

#endif /* ifndef RED_BLACK_FLATTEN_H_ */
