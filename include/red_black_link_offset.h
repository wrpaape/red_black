#ifndef RED_BLACK_LINK_OFFSET_H_
#define RED_BLACK_LINK_OFFSET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h" /* RedBlackNode */


/* get NODE->left|right depending on OFFSET
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_LINK_OFFSET(NODE,					\
			      OFFSET)					\
((struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFFSET)))

#endif /* ifndef RED_BLACK_LINK_OFFSET_H_ */
