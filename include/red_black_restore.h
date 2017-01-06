#ifndef RED_BLACK_RESTORE_H_
#define RED_BLACK_RESTORE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_node.h" /* RedBlackNode, bool */
#include "red_black_jump.h" /* JumpBuffer */

/* external API
 * ────────────────────────────────────────────────────────────────────────── */
/* immediate restore of red node deletion/removal: always succeeds */
void
red_black_restore_red(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict lchild,
		      struct RedBlackNode *restrict rchild);

/* immediate restore of black node deletion/removal: true/false success/fail */
bool
red_black_restore_black(struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNode *const restrict lchild,
			struct RedBlackNode *restrict rchild);

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
