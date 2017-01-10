#ifndef RED_BLACK_CONCAT_H_
#define RED_BLACK_CONCAT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* struct RedBlackNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* turns a binary tree 'node' into a list (node->left->left-> ... left->NULL)
 * if not NULL, concats 'node' with input 'end_ptr' returns a pointer to the
 * last 'left' link */
struct RedBlackNode *restrict *restrict
red_black_concat(struct RedBlackNode *restrict node,
		 struct RedBlackNode *restrict *restrict end_ptr);

#endif /* ifndef RED_BLACK_CONCAT_H_ */
