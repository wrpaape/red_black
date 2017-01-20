#ifndef RED_BLACK_RESTORE_H_
#define RED_BLACK_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node_factory.h" /* Node, NodeFactory, and JumpBuffer */
#include "red_black_jump.h"	    /* JumpBuffer */

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* initial restore points */
void
red_black_restore_root(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNode *const restrict root,
		       struct RedBlackNodeFactory *const restrict factory);
void
red_black_restore_min_root(struct RedBlackNode *restrict *const restrict tree,
			   struct RedBlackNode *const restrict root,
			   struct RedBlackNodeFactory *const restrict factory);
void
red_black_restore_max_root(struct RedBlackNode *restrict *const restrict tree,
			   struct RedBlackNode *const restrict root,
			   struct RedBlackNodeFactory *const restrict factory);

void
red_black_restore_node(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNode *const restrict node,
		       struct RedBlackNodeFactory *const restrict factory,
		       RedBlackJumpBuffer *const restrict jump_buffer);
void
red_black_restore_min_node(struct RedBlackNode *restrict *const restrict tree,
			   struct RedBlackNode *const restrict node,
			   struct RedBlackNodeFactory *const restrict factory,
			   RedBlackJumpBuffer *const restrict jump_buffer);
void
red_black_restore_max_node(struct RedBlackNode *restrict *const restrict tree,
			   struct RedBlackNode *const restrict node,
			   struct RedBlackNodeFactory *const restrict factory,
			   RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_restore_l_bot(struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNode *const restrict parent,
			RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_restore_l_mid(struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNode *const restrict parent,
			RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_restore_r_bot(struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNode *const restrict parent,
			RedBlackJumpBuffer *const restrict jump_buffer);

void
red_black_restore_r_mid(struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNode *const restrict parent,
			RedBlackJumpBuffer *const restrict jump_buffer);

#endif /* ifndef RED_BLACK_RESTORE_H_ */
