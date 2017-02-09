#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hkey.h" /* RedBlackHKey */
#include <stdbool.h>                       /* bool */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHNode {
	struct RedBlackHKey hkey;
	bool is_red;
	struct RedBlackHNode *restrict left;
	struct RedBlackHNode *restrict right;
};

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HNODE_H_ */
