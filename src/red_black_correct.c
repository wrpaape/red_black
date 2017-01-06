#include "red_black_correct.h" /* RedBlackNode, JumpBuffer */

void
red_black_correct_ll_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		parent->right = grandparent;

		grandparent->is_red = true;
		grandparent->left   = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

void
red_black_correct_ll_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle->is_red) { /* uncle can't be NULL */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		grandparent->is_red = true;
		grandparent->left   = parent->right;
		parent->right       = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}


void
red_black_correct_lr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left-right, recolor node and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = node;

		node->is_red = false;
		node->left   = parent;
		node->right  = grandparent;

		parent->right = NULL;

		grandparent->is_red = true;
		grandparent->left   = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

void
red_black_correct_lr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->right;

		if (uncle->is_red) { /* uncle cannot be NULL */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left-right, recolor node and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = node;

		node->is_red  = false;
		parent->right = node->left;
		node->left    = parent;

		grandparent->is_red = true;
		grandparent->left   = node->right;
		node->right         = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

void
red_black_correct_rr_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		parent->left = grandparent;

		grandparent->is_red = true;
		grandparent->right  = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

void
red_black_correct_rr_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		parent->is_red = false;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle->is_red) { /* uncle can't be NULL */
			uncle->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate left, recolor grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = parent;

		grandparent->is_red = true;
		grandparent->right  = parent->left;
		parent->left        = grandparent;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}


void
red_black_correct_rl_bot(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle != NULL) { /* uncle must be RED, recolor */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right-left, recolor node and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = node;

		node->is_red = false;
		node->left   = grandparent;
		node->right  = parent;

		parent->left = NULL;

		grandparent->is_red = true;
		grandparent->right  = NULL;
	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}

void
red_black_correct_rl_mid(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict grandparent,
			 struct RedBlackNode *const restrict parent,
			 struct RedBlackNode *const restrict node,
			 RedBlackJumpBuffer *const restrict jump_buffer)
{
	if (parent->is_red) {
		struct RedBlackNode *const restrict uncle = grandparent->left;

		if (uncle->is_red) { /* uncle cannot be NULL */
			uncle->is_red  = false;
			parent->is_red = false;
			return; /* need to propogate recolor */
		}

		/* rotate right-left, recolor node and grandparent
		 * ────────────────────────────────────────────────────────── */
		*tree = node;

		node->is_red        = false;
		grandparent->is_red = true;
		grandparent->right  = node->left;
		node->left          = grandparent;

		parent->left = node->right;
		node->right  = parent;

	}

	RED_BLACK_JUMP_3_TRUE(jump_buffer); /* all done, node was inserted */
}
