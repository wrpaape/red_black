#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONCAT_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONCAT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h" /* struct RedBlackHNode */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* turns a binary tree 'node' into a list (node->left->left-> ... left->NULL)
 * if not NULL, concats 'node' with input 'end_ptr' returns a pointer to the
 * last 'left' link */
struct RedBlackHNode *restrict *restrict
red_black_hconcat(struct RedBlackHNode *const restrict node,
		  struct RedBlackHNode *restrict *restrict end_ptr);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCONCAT_H_ */
