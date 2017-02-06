#ifndef RED_BLACK_ITOR_NODE_H_
#define RED_BLACK_ITOR_NODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h" /* RedBlackNode */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackItorNode {
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict node;
	int parent_info;
};

#endif /* ifndef RED_BLACK_ITOR_NODE_H_ */
