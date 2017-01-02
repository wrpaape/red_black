#ifndef RED_BLACK_NODE_H_
#define RED_BLACK_NODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdbool.h>   /* bool */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNode {
	const void *key;
	bool is_red;
	struct RedBlackNode *restrict left;
	struct RedBlackNode *restrict right;
};

#endif /* ifndef RED_BLACK_NODE_H_ */
