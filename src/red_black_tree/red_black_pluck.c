#include "red_black_tree/red_black_pluck.h"	    /* types, JUMP API */
#include "red_black_tree/red_black_restore.h"	    /* restore API */
#include "red_black_common/red_black_stack_count.h" /* RED_BLACK_STACK_COUNT */


/* typedefs
 * ────────────────────────────────────────────────────────────────────────── */
typedef void
(*RedBlackPluckNode)(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict parent,
		     const RedBlackComparator comparator,
		     struct RedBlackNodeFactory *const restrict factory,
		     RedBlackJumpBuffer jump_buffer,
		     const void *const key,
		     void **const restrict pluck_ptr);

/* pluck state machine functions
 *
 * JUMPS
 *	RED_BLACK_JUMP_VALUE_3_TRUE	OK, successful deletion, tree updated
 * 	RED_BLACK_JUMP_VALUE_3_FALSE	OK, no deletion made, tree NOT updated
 *
 * RETURNS
 *	need to restore (check if can rotate, recolor) in THIS frame
 * ────────────────────────────────────────────────────────────────────────── */
static void
rb_pluck_l(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr);
static void
rb_pluck_r(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr);

static void
rb_pluck_l(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *restrict *const restrict subtree = &parent->left;
	struct RedBlackNode *const restrict lnode	      = *subtree;

	const int compare = comparator(key,
				       lnode->key);

	if (compare == 0) {
		*pluck_ptr = (void *) lnode->key;

		red_black_restore_node(subtree,
				       lnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_l_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(subtree,
			   lnode,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_l_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}

static void
rb_pluck_r(struct RedBlackNode *restrict *const restrict tree,
	   struct RedBlackNode *const restrict parent,
	   const RedBlackComparator comparator,
	   struct RedBlackNodeFactory *const restrict factory,
	   RedBlackJumpBuffer jump_buffer,
	   const void *const key,
	   void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *restrict *const restrict subtree = &parent->right;
	struct RedBlackNode *const restrict rnode	      = *subtree;

	const int compare = comparator(key,
				       rnode->key);

	if (compare == 0) {
		*pluck_ptr = (void *) rnode->key;

		red_black_restore_node(subtree,
				       rnode,
				       factory,
				       jump_buffer);

		/* if returned, need to restore */
		red_black_restore_r_bot(tree,
					parent,
					jump_buffer);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(subtree,
			   rnode,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);

		/* if returned, need to restore */
		red_black_restore_r_mid(tree,
					parent,
					jump_buffer);
	}
	/* if returned, previous frame needs to restore */
}



void
red_black_pluck(struct RedBlackNode *restrict *const restrict tree,
		const RedBlackComparator comparator,
		struct RedBlackNodeFactory *const restrict factory,
		RedBlackJumpBuffer jump_buffer,
		const void *const key,
		void **const restrict pluck_ptr)
{
	RedBlackPluckNode next_pluck;

	struct RedBlackNode *const restrict root = *tree;

	const int compare = comparator(key,
				       root->key);

	if (compare == 0) {
		*pluck_ptr = (void *) root->key;

		red_black_restore_root(tree,
				       root,
				       factory);

	} else {
		next_pluck = (compare < 0)
			   ? &rb_pluck_l
			   : &rb_pluck_r;

		next_pluck(tree,
			   root,
			   comparator,
			   factory,
			   jump_buffer,
			   key,
			   pluck_ptr);
	}
}


void
red_black_pluck_min(struct RedBlackNode *restrict *const restrict tree,
		    struct RedBlackNodeFactory *const restrict factory,
		    RedBlackJumpBuffer jump_buffer,
		    void **const restrict pluck_ptr)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict next;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;

	struct RedBlackNode *const restrict root = *tree;

	node = root->left;

	if (node == NULL) {
		*pluck_ptr = (void *) root->key;

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

		*pluck_ptr = (void *) node->key;

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
				/* if returned, continue unwinding stack */
			}

			/* restore root */
			(void) rb_restore_l_mid_b(tree,
						  root);
		}
	}
}

void
red_black_pluck_max(struct RedBlackNode *restrict *const restrict tree,
		    struct RedBlackNodeFactory *const restrict factory,
		    RedBlackJumpBuffer jump_buffer,
		    void **const restrict pluck_ptr)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict next;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict cursor;

	struct RedBlackNode *const restrict root = *tree;

	node = root->right;

	if (node == NULL) {
		*pluck_ptr = (void *) root->key;

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

		*pluck_ptr = (void *) node->key;

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
