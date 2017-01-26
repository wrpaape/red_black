#ifndef RED_BLACK_LINK_OFFSET_H_
#define RED_BLACK_LINK_OFFSET_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h" /* RedBlackNode */
#include <stddef.h>	    /* size_t, offsetof() */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackLinkOffset {
	size_t next;
	size_t prev;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern const struct RedBlackLinkOffset asc_link_offset;
extern const struct RedBlackLinkOffset desc_link_offset;


/* macros
 * ────────────────────────────────────────────────────────────────────────── */
/* get node->left|right depending on OFFSET */
#define RBLO_GET_NODE(NODE,						\
		      OFFSET)						\
*((struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFFSET)))

#endif /* ifndef RED_BLACK_LINK_OFFSET_H_ */
