#include "red_black_delete.h"	   /* Comparator, Node, NodeFactory, JUMP */
#include "red_black_restore.h"	   /* restore API */
#include "red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackDeleteNode)(struct RedBlackNode *restrict *const restrict tree,
		      struct RedBlackNode *const restrict parent,
		      const RedBlackComparator comparator,
		      struct RedBlackNodeFactory *const restrict factory,
		      RedBlackJumpBuffer *const restrict jump_buffer,
		      const void *const key);

/* delete state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_2_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_2_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);
static void
rb_delete_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key);

static void
rb_delete_l(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict lnode = parent->left;

	if (lnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       lnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_delete = (compare < 0)
			    ? &rb_delete_l
			    : &rb_delete_r;

		next_delete(subtree,
			    lnode,
			    comparator,
			    factory,
			    jump_buffer,
			    key);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_delete_r(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    const RedBlackComparator comparator,
	    struct RedBlackNodeFactory *const restrict factory,
	    RedBlackJumpBuffer *const restrict jump_buffer,
	    const void *const key)
{
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict rnode = parent->right;

	if (rnode == NULL)
		RED_BLACK_JUMP_2_FALSE(jump_buffer); /* done, no update */

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		red_black_restore_node(subtree,
				       rnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_delete = (compare < 0)
			    ? &rb_delete_l
			    : &rb_delete_r;

		next_delete(subtree,
			    rnode,
			    comparator,
			    factory,
			    jump_buffer,
			    key);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



int
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const RedBlackComparator comparator,
		 struct RedBlackNodeFactory *const restrict factory,
		 RedBlackJumpBuffer *const restrict jump_buffer,
		 const void *const key)
{
	int status;
	RedBlackDeleteNode next_delete;

	struct RedBlackNode *const restrict root = *tree;

	status = (root != NULL);

	if (status) {
		const int compare = comparator(key,
					       root->key);

		status = (compare == 0);

		if (status) {
			red_black_restore_root(tree,
					       root,
					       factory);

		} else {
			next_delete = (compare < 0)
				    ? &rb_delete_l
				    : &rb_delete_r;

			next_delete(tree,
				    root,
				    comparator,
				    factory,
				    jump_buffer,
				    key);

			return 1; /* updated */
		}
	}

	return status;
}


int
red_black_delete_min(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict next;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;

	struct RedBlackNode *const restrict root = *tree;

	const int status = (root != NULL);

	if (status) {
		node = root->left;

		if (node == NULL) {
			red_black_restore_min_root(tree,
						   root,
						   factory);

		} else {
			cursor  = &stack[0];
			*cursor = root;

			/* find min node */
			while (1) {
				next = node->left;

				if (next == NULL)
					break;

				++cursor;
				*cursor = node;

				node = next;
			}

			parent = *cursor;

			red_black_restore_min_node(&parent->left,
						   node,
						   factory,
						   jump_buffer);
			/* if returned, need to restore */

			if (parent == root) {
				/* restore root */
				(void) rb_restore_l_bot_b(tree,
							  root);

			} else {
				node = parent;

				--cursor;
				parent = *cursor;

				red_black_restore_l_bot(&parent->left,
							node,
							jump_buffer);

				while (parent != root) {
					node = parent;

					--cursor;
					parent = *cursor;

					red_black_restore_l_mid(&parent->left,
								node,
								jump_buffer);
					/* if returned, unwind stack */
				}
				/* restore root */
				(void) rb_restore_l_mid_b(tree,
							  root);
			}
		}
	}

	return status; /* 1, 0 (deleted, untouched) */
}


int
red_black_delete_max(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer *const restrict jump_buffer)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict next;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;

	struct RedBlackNode *const restrict root = *tree;

	const int status = (root != NULL);

	if (status) {
		node = root->right;

		if (node == NULL) {
			red_black_restore_max_root(tree,
						   root,
						   factory);

		} else {
			cursor  = &stack[0];
			*cursor = root;

			/* find max node */
			while (1) {
				next = node->right;

				if (next == NULL)
					break;

				++cursor;
				*cursor = node;

				node = next;
			}

			parent = *cursor;

			red_black_restore_max_node(&parent->right,
						   node,
						   factory,
						   jump_buffer);

			/* if returned, need to restore */
			if (parent == root) {
				/* restore root */
				(void) rb_restore_r_bot_b(tree,
							  root);

			} else {
				node = parent;

				--cursor;
				parent = *cursor;

				red_black_restore_r_bot(&parent->right,
							node,
							jump_buffer);

				while (parent != root) {
					node = parent;

					--cursor;
					parent = *cursor;

					red_black_restore_r_mid(&parent->right,
								node,
								jump_buffer);
					/* if returned, continue unwinding stack */
				}

				/* restore root */
				(void) rb_restore_r_mid_b(tree,
							  root);
			}
		}
	}

	return status; /* 1, 0 (deleted, untouched) */
}
