#ifndef RED_BLACK_HNODE_H_
#define RED_BLACK_HNODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h"	/* RedBlackNode, bool */
#include "red_black_hkey.h"	/* RedBlackHKey */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHNode {
	struct RedBlackNode node;
	struct RedBlackHKey hkey;
};

#endif /* ifndef RED_BLACK_HNODE_H_ */
