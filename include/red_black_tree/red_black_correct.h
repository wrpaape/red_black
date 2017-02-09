#ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_CORRECT_H_
#define RED_BLACK_RED_BLACK_TREE_RED_BLACK_CORRECT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree/red_black_node.h"   /* RedBlackNode */
#include "red_black_common/red_black_jump.h" /* JumpBuffer */


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* insertion correction functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, tree updated
 *
 * RETURNS
 *	recolor in PREV stack frame, then correct in PREV-PREV frame
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_correct_ll_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_ll_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_lr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_lr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_rr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_rr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_rl_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer jump_buffer);
void
red_black_correct_rl_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_TREE_RED_BLACK_CORRECT_H_ */
