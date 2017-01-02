#ifndef RED_BLACK_NODE_H_
#define RED_BLACK_NODE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdbool.h>   /* bool */

#include <stdio.h>
#define DEBUG(...) do { printf(__VA_ARGS__); fflush(stdout); } while (0)

/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackNode {
	const void *key;
	bool is_red;
	struct RedBlackNode *restrict left;
	struct RedBlackNode *restrict right;
};

#endif /* ifndef RED_BLACK_NODE_H_ */
