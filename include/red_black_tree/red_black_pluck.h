#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PLUCK_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_PLUCK_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_comparator.h"   /* Comparator */
#include "red_black_tree/red_black_node_factory.h" /* Node, NodeFactory, JUMP */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_pluck(struct RedBlackNode *restrict *const restrict tree,
		const RedBlackComparator comparator,
		struct RedBlackNodeFactory *const restrict factory,
		RedBlackJumpBuffer jump_buffer,
		const void *const key,
		void **const restrict pluck_ptr);

void
red_black_pluck_min(struct RedBlackNode *restrict *const restrict tree,
		    struct RedBlackNodeFactory *const restrict factory,
		    RedBlackJumpBuffer jump_buffer,
		    void **const restrict pluck_ptr);

void
red_black_pluck_max(struct RedBlackNode *restrict *const restrict tree,
		    struct RedBlackNodeFactory *const restrict factory,
		    RedBlackJumpBuffer jump_buffer,
		    void **const restrict pluck_ptr);

/* may jump or return after successful deletion
 * ────────────────────────────────────────────────────────────────────────── */

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_PLUCK_H_ */
