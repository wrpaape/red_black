#ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCORRECT_H_
#define RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCORRECT_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap/red_black_hnode.h"  /* RedBlackHNode */
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
red_black_hcorrect_ll_bot(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_ll_mid(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_lr_bot(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  struct RedBlackHNode *const restrict node,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_lr_mid(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  struct RedBlackHNode *const restrict node,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_rr_bot(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_rr_mid(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_rl_bot(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  struct RedBlackHNode *const restrict node,
			  RedBlackJumpBuffer jump_buffer);
void
red_black_hcorrect_rl_mid(struct RedBlackHNode *restrict *const restrict tree,
			  struct RedBlackHNode *const restrict grandparent,
			  struct RedBlackHNode *const restrict parent,
			  struct RedBlackHNode *const restrict node,
			  RedBlackJumpBuffer jump_buffer);

#endif /* ifndef RED_BLACK_RED_BLACK_HMAP_RED_BLACK_HCORRECT_H_ */
