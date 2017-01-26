#include "red_black_link_offset.h" /* LinkOffset, Node, offsetof() */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const struct RedBlackLinkOffset asc_link_offset = {
	.next = offsetof(struct RedBlackNode, right),
	.prev = offsetof(struct RedBlackNode, left)
};

const struct RedBlackLinkOffset desc_link_offset = {
	.next = offsetof(struct RedBlackNode, left),
	.prev = offsetof(struct RedBlackNode, right)
};
