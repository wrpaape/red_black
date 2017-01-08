#ifndef RED_BLACK_HASH_NODE_H_
#define RED_BLACK_HASH_NODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* RedBlackNode, bool */
#include "red_black_hash_key.h"	/* RedBlackHashKey */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHashNode {
	struct RedBlackNode node;
	struct RedBlackHashKey hash_key;
};

#endif /* ifndef RED_BLACK_HASH_NODE_H_ */
