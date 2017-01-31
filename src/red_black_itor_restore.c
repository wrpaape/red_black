#include "red_black_itor_restore.h" /* Node, NodeFactory, Itor types */
#include "red_black_stack_count.h"  /* RED_BLACK_STACK_COUNT */
#include <stddef.h>		    /* NULL */


static inline bool
rbi_restore_black(struct RedBlackItorCursor *const restrict cursor,
		  struct RedBlackItorNode *restrict *const restrict itor_node,
		  struct RedBlackNode *restrict *restrict tree,
		  struct RedBlackNode *const restrict lchild,
		  struct RedBlackNode *restrict rchild)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	bool restored;

	struct RedBlackNode *restrict replacement_stack[RED_BLACK_STACK_COUNT];
	struct RedBlackNode *restrict *restrict replacement_cursor;


	tree = itor_node->tree

	if (lchild == NULL) {
		*tree = rchild;

		restored = (rchild != NULL);
		if (restored)
			rchild->is_red = false; /* rchild RED leaf -> restore */

		return restored;

	} else if (rchild == NULL) {
		*tree = lchild;
		lchild->is_red = false;
		return true; /* lchild must be RED leaf, -> BLACK -> restored */
	}

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->replacement_parent->replacement
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) /* black height of 1 or 2 */
		return rb_restore_black_shallow(tree,
						lchild,
						rchild);

	replacement_cursor  = &replacement_stack[0];
	*replacement_cursor = NULL;

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		++replacement_cursor;
		*replacement_cursor = replacement_parent;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	restored = rb_restore_black_rtree(&rchild,
					  replacement_cursor,
					  replacement_parent,
					  replacement,
					  replacement_child);

	replacement->right = rchild; /* set right subtree of replacement */

	if (restored) {
		/* black height restored through recoloring and/or rotation of
		 * ancestors or right child of replacement
		 *
		 * replacement guaranteed BLACK */
		*tree = replacement;
		replacement->left = lchild;
		return true;
	}

	/* replacement is BLACK, left subtree of replacement is still NULL
	 *
	 * right subtree of replacement is valid (balanced) but deficient 1
	 * black height
	 *
	 * right subtree of replacement has BLACK root (rchild) and black
	 * height of AT LEAST 2 (inclusive)
	 *
	 * lchild has black height of AT LEAST 3 (inclusive)
	 * ────────────────────────────────────────────────────────────────── */
	return rb_restore_black_ltree(tree,
				      lchild,
				      replacement);
}


void
red_black_itor_restore_root(struct RedBlackItorCursor *const restrict cursor,
			    struct RedBlackItorNode *const restrict itor_root
			    struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackNode *restrict root;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	tree = itor_root->tree;
	root = itor_root->node;

	lchild = root->left;
	rchild = root->right;

	/* free node */
	rbnf_free(factory,
		  root);

	(void) rbi_restore_black(cursor,
				 itor_root,
				 tree,
				 lchild,
				 rchild);
}
