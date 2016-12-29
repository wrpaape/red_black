#include "red_black_delete.h"

#define RSTACK_COUNT 81 /* can handle trees of upto at least 1,000,000,000 nodes */

static inline bool
rb_replace_black_shallow(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict lnode,
			 struct RedBlackNode *const restrict rnode)
{

	struct RedBlackNode *restrict llchild;
	struct RedBlackNode *restrict lrchild;
	struct RedBlackNode *restrict lrrgrandchild;
	struct RedBlackNode *restrict rrchild; /* rlchild is NULL */
	bool restored;

	if (rnode->is_red) {
		/* rnode RED, lnode RED -> black height of 1
		 * ────────────────────────────────────────────────────────── */
		rnode->is_red = false;
		rnode->left   = lnode;

		*tree = rnode;
		return true;
	}

	/* lnode, rnode subtrees have black height of 2
	 * ────────────────────────────────────────────────────────────────── */
	rrchild = rnode->right;

	if (rrchild != NULL) {
		rrchild->is_red = false; /* must be RED */

		rnode->left = lnode;

		*tree = rnode;
		return true;
	}

	/* rnode is BLACK leaf
	 * ────────────────────────────────────────────────────────────────── */
	lrchild = lnode->right;

	if (lrchild == NULL) {
		lnode->right = rnode; /* lnode is BLACK */

		llchild = lnode->left;

		/* only non-restorable case: lnode and rnode are BLACK leaves
		 * ────────────────────────────────────────────────────────── */
		restored = (llchild != NULL);
		if (restored)
			llchild->is_red = false;
		else
			rnode->is_red = true; /* restore balance */

		*tree = lnode;
		return restored;

	/* RED node exists somewhere to be recolored, always restorable
	 * ────────────────────────────────────────────────────────────────── */
	} else if (lrchild->is_red) {
		/* lnode is BLACK, lrchild is RED leaf
		 * ────────────────────────────────────────────────────────── */
		lrchild->is_red = false;
		lrchild->left   = lnode;
		lrchild->right  = rnode;

		lnode->right = NULL;

		*tree = lrchild;

	} else {
		/* lnode is RED w/ 2 BLACK children, set BLACK
		 * ────────────────────────────────────────────────────────── */
		lnode->is_red = false;

		lrrgrandchild = lrchild->right;

		if (lrrgrandchild == NULL) {
			lrchild->right = rnode;

			rnode->is_red = true; /* make node new RED max leaf */

		} else {
			lrchild->right = NULL;

			lrrgrandchild->left  = lrchild;
			lrrgrandchild->right = rchild;

			lchild->right = lrrgrandchild;
		}

		*tree = lnode;
	}

	return true;
}


static inline void
rb_replace_red_shallow(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNode *const restrict lnode,
		       struct RedBlackNode *const restrict rnode)
{
	struct RedBlackNode *restrict lrchild;
	struct RedBlackNode *restrict rrchild;

	lrchild = lnode->right;

	if (lrchild == NULL) {
		rrchild = rnode->right;

		if (rrchild == NULL) {
			*tree = lnode;

			lnode->right = rnode;

			rnode->is_red = true;

		} else {
			*tree = rnode;

			rnode->is_red = true;
			rnode->left   = lnode;

			rrchild->is_red = false;
		}

	} else { /* lrchild MUST be RED leaf */
		*tree = lrchild;

		lrchild->left  = lnode;
		lrchild->right = rnode;
	}
}


/* unwind min successor stack to attempt to restore black height and balance
 * if black height cannot be restored, retore balance and return false */
static inline bool
rb_replace_rtree_unwind(struct RedBlackNode *restrict *restrict root,
			struct RedBlackNode *const restrict *restrict stack_ptr,
			struct RedBlackNode *restrict parent)
{
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict rnode;
	struct RedBlackNode *restrict rlchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict rllgrandchild;
	struct RedBlackNode *restrict rlrgrandchild;

	while (parent != NULL) {
		--stack_ptr;
		grandparent = *stack_ptr;

		rnode   = parent->right;
		rlchild = rnode->left;

		if (rnode->is_red) {
			/* parent is BLACK, rnode is RED
			 * ────────────────────────────────────────────────── */
			rnode->is_red = false;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = rnode; /* rnode is the new root */

			/* rnode->left MAY be replaced */

			rllgrandchild = rlchild->left;
			rlrgrandchild = rlchild->right;

			if (rlrgrandchild->is_red) {
				/* no need to update rlchild */
				rlrgrandchild->is_red = false;

				/* rlchild must have been BLACK */
				rlchild->is_red = true;
				rlchild->left   = parent;

				parent->right = rllgrandchild;

			} else if (rllgrandchild->is_red) {
				/* make rllgrandchild new rlchild */
				rnode->left = rllgrandchild;

				parent->right       = rllgrandchild->left;
				rllgrandchild->left = parent;

				rlchild->left        = rllgrandchild->right;
				rllgrandchild->right = rlchild;

			} else {
				/* rlchild is root, no need to update */
				rlchild->left = parent;

				/* parent and parent->left were BLACK */
				parent->is_red = true;
				parent->right  = rllgrandchild;
			}

			return true; /* always completely restorable */

		} else if (parent->is_red) {
			/* parent is RED, rnode is BLACK
			 * ────────────────────────────────────────────────── */
			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			/* root will ALWAYS be replaced */
			if (rlchild->is_red) {
				*root = rlchild; /* rlchild is new root */

				parent->is_red = false;
				parent->right  = rlchild->left;
				rlchild->left  = parent;

				rnode->left    = rlchild->right;
				rlchild->right = rnode;

			} else {
				*root = rnode; /* rnode is new root */

				rnode->left = parent;

				parent->right = rlchild;
			}

			return true; /* always completely restorable */

		/* parent is BLACK, rnode is BLACK
		 * ────────────────────────────────────────────────────────── */
		} else if (rlchild->is_red) {
			rlchild->is_red = false;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = rlchild; /* rlchild is new root */

			parent->right = rlchild->left;
			rlchild->left = parent;

			rnode->left    = rlchild->right;
			rlchild->right = rnode;

			return true; /* always completely restorable */

		} else {
			rchild = rnode->right;

			if (rchild->is_red) {
				rchild->is_red = false;

				if (grandparent != NULL) /* parent != root */
					root = &grandparent->left;

				*root = rnode; /* rnode is new root */

				rnode->left = parent;

				parent->right = rlchild;

				return true; /* always completely restorable */
			}
		}
		/* parent, lnode (parent->left), rnode (parent->right),
		 * rlchild (rnode->left), and rchild (rnode->right) are ALL
		 * BLACK
		 *
		 * recolor rnode to restore balance in parent tree, but still
		 * deficient 1 black height -> recurse */
		rnode->is_red = true;

		parent = grandparent;
	}
	/* unwound all the way to root
	 * balance is restored, but black height is still deficient */
	return false;
}


static inline bool
rb_replace_rtree(struct RedBlackNode *restrict *restrict root,
		 struct RedBlackNode *const restrict *restrict stack_ptr,
		 struct RedBlackNode *restrict parent)
{
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict rnode;
	struct RedBlackNode *restrict rlchild;
	struct RedBlackNode *restrict rrchild;
	struct RedBlackNode *restrict rllgrandchild;

	grandparent = *stack_ptr;
	rnode       = parent->right;
	rlchild     = rnode->left;

	if (parent->is_red) {
		parent->is_red = false;

		/* rnode must be BLACK */
		if (rlchild == NULL) {
			rnode->is_red = true;
			rrchild = rnode->right;

			if (rrchild != NULL) {
				/* rrchild must be RED leaf */
				rrchild->is_red = false;

				if (grandparent != NULL)
					root = &grandparent->left;

				*root = rnode;

				rnode->left = parent;

				parent->right = NULL;
			} /* else no need to change root */

		} else {
			if (grandparent != NULL)
				root = &grandparent->left;

			*root = rlchild;

			rlchild->left  = parent;
			rlchild->right = rnode;

			parent->right = NULL;

			rnode->left = NULL;
		}

	} else if (rnode->is_red) {
		 /* can balance AND restore black height */
		parent->right = NULL;

		if (grandparent != NULL)
			root = &grandparent->left;

		/* rlchild is BLACK, not NULL */
		rllgrandchild = rlchild->left;

		if (rllgrandchild == NULL) {
			rlchild->left = parent; /* put parent on min */

			parent->is_red = true;

			*root = rnode;

			rnode->is_red = false;

		} else {
			rlchild->left = NULL; /* bring ggchild to root */

			*root = rllgrandchild;

			rllgrandchild->is_red = false;
			rllgrandchild->left   = parent;
			rllgrandchild->right  = rnode;
		}

	} else if (rlchild == NULL) {
		rrchild = rnode->right;

		if (rrchild == NULL) {
			rnode->is_red = true;
			/* grandparent->parent->rnode balanced, but are
			 * deficient 1 black height -> unwind stack to
			 * look for opportunity to correct
			 * ────────────────────────────────────────── */
			return rb_replace_rtree_unwind(root,
						       stack_ptr,
						       grandparent);
		}

		/* rrchild must be RED leaf */
		rrchild->is_red = false;

		if (grandparent != NULL)
			root = &grandparent->left;

		*root = rnode;

		rnode->left = parent;

		parent->right = NULL;

	} else {
		/* rlchild is a RED leaf, make new root */
		if (grandparent != NULL)
			root = &grandparent->left;

		*root = rlchild;

		rlchild->is_red = false;
		rlchild->left   = parent;
		rlchild->right  = rnode;

		parent->right = NULL;

		rnode->left = NULL;
	}

	return true;
}


/* attempt to restore black height in the replacement node (lnode) tree (right
 * subtree of deleted BLACK node)
 *
 * if can't, restore balance and return false */
static inline bool
rb_replace_black_rtree(struct RedBlackNode *restrict *restrict root,
		       struct RedBlackNode *const restrict *restrict stack_ptr,
		       struct RedBlackNode *restrict parent,
		       struct RedBlackNode *const restrict lnode,
		       struct RedBlackNode *const restrict lrchild)
{
	if (lrchild != NULL) /* replacement is BLACK, child is RED leaf */
		lrchild->is_red = false;
	else if (lnode->is_red) /* ensure replacement BLACK */
		lnode->is_red   = false;
	else
		return rb_replace_rtree(root,
					stack_ptr,
					parent);

	return true; /* completely restored */
}

static inline bool
rb_replace_red_rtree(struct RedBlackNode *restrict *restrict root,
		     struct RedBlackNode *const restrict *restrict stack_ptr,
		     struct RedBlackNode *restrict parent,
		     struct RedBlackNode *const restrict lnode,
		     struct RedBlackNode *const restrict lrchild)
{

	if (!lnode->is_red) {
		lnode->is_red = true;

		if (lrchild == NULL)
			return rb_replace_rtree(root,
						stack_ptr,
						parent);

		lrchild->is_red = false;
	}

	return true; /* completely restored */
}


/* search upto 2 nodes deep into left subtree of deleted node
 * for RED nodes -> opportunity to restored balance and black height
 * after call to rb_replace_black_rtree */
static inline bool
rb_replace_black_ltree(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNode *const restrict lnode,
		       struct RedBlackNode *const restrict rnode)
{
	struct RedBlackNode *restrict llchild;
	struct RedBlackNode *restrict lrchild;
	struct RedBlackNode *restrict lrlgrandchild;
	struct RedBlackNode *restrict lrrgrandchild;

	lrchild = lnode->right;

	if (lnode->is_red) {
		lrlgrandchild = lrchild->left;
		lrrgrandchild = lrchild->right;

		if (lrlgrandchild->is_red) {
			if (lrrgrandchild->is_red) {
				*tree = lrchild; /* must be BLACK */

				lrchild->left  = lnode;
				lrchild->right = rnode;

				lnode->right = lrlgrandchild;

				rnode->left = lrrgrandchild;

				lrlgrandchild->is_red = false;

			} else {
				*tree = lnode;

				lnode->is_red = false;

				lrchild->right = rnode;

				rnode->is_red = true;
				rnode->left   = lrrgrandchild;
			}

		} else {
			*tree = lnode;
			lnode->is_red = false;

			if (lrrgrandchild->is_red) {
				lrrgrandchild->is_red = false;

				lrchild->is_red     = true;
				lrchild->right      = lrrgrandchild->left;
				lrrgrandchild->left = lrchild;

				rnode->is_red        = true;
				rnode->left          = lrrgrandchild->right;
				lrrgrandchild->right = rnode;

				lnode->right = lrrgrandchild;

			} else {
				lnode->right = rnode;

				rnode->left = lrchild;

				lrchild->is_red = true;
			}
		}

	} else if (lrchild->is_red) {
		lrchild->is_red = false;

		lnode->right  = lrchild->left;
		lrchild->left = lnode;

		rnode->left    = lrchild->right;
		lrchild->right = rnode;

		*tree = lrchild;

	} else {
		llchild = lnode->left;

		if (llchild->is_red) {
			llchild->is_red = false;

			*tree = lnode;

			lnode->right = rnode;

			rnode->left = lrchild;

		} else {
			*tree = rnode;

			rnode->left = lnode;

			lnode->is_red = true;

			/* balanced, but still deficient 1 black height
			 * ────────────────────────────────────────────────── */
			return false;
		}
	}

	/* balance and black height restored to pre-deletion state
	 * ────────────────────────────────────────────────────────────────── */
	return true;
}


static inline void
rb_replace_red_ltree(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict lnode,
		     struct RedBlackNode *const restrict rnode)
{
	struct RedBlackNode *restrict lrchild;

	lrchild = lnode->right;

	if (lrchild->is_red) {
		lnode->right  = lrchild->left;
		lrchild->left = lnode;

		rnode->is_red  = false; /* rnode must have been red */
		rnode->left    = lrchild->right;
		lrchild->right = rnode;

		*tree = lrchild;

	} else {
		*tree = lnode;

		lnode->right = rnode;

		rnode->left  = lrchild;
	}
}


static inline bool
rb_replace_black(struct RedBlackNode *restrict *const restrict tree,
		 const struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	bool restored;

	struct RedBlackNode *const restrict replacement_stack[RSTACK_COUNT];
	struct RedBlackNode *const restrict *restrict replacement_stack_ptr;

	lchild = node->left;
	rchild = node->right;

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
		return rb_replace_black_shallow(tree,
						lchild,
						rchild);

	replacement_stack_ptr  = &replacement_stack[0];
	*replacement_stack_ptr = NULL;

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		++replacement_stack_ptr;
		*replacement_stack_ptr = replacement_parent;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	restored = rb_replace_black_rtree(&rchild,
					  replacement_stack_ptr,
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
	return rb_replace_black_ltree(tree,
				      lchild,
				      replacement);
}

static inline void
rb_replace_red(struct RedBlackNode *restrict *const restrict tree,
	       const struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	bool restored;

	struct RedBlackNode *const restrict replacement_stack[RSTACK_COUNT];
	struct RedBlackNode *const restrict *restrict replacement_stack_ptr;

	lchild = node->left;

	if (lchild == NULL) {
		*tree = NULL; /* rchild must be NULL */
		return;
	}

	rchild = node->right;

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->replacement_parent->replacement
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		rb_replace_red_shallow(tree,
				       lchild,
				       rchild);
		return;
	}

	replacement_stack_ptr  = &replacement_stack[0];
	*replacement_stack_ptr = NULL;

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		++replacement_stack_ptr;
		*replacement_stack_ptr = replacement_parent;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	restored = rb_replace_red_rtree(&rchild,
					replacement_stack_ptr,
					replacement_parent,
					replacement,
					replacement_child);

	replacement->right = rchild; /* set right subtree of replacement */

	if (restored) {
		/* black height restored through recoloring and/or rotation of
		 * ancestors or right child of replacement
		 *
		 * replacement is RED */
		*tree = replacement;
		replacement->left = lchild;

	} else {
		rb_replace_red_ltree(tree,
				     lchild,
				     replacement);
	}
}

static inline bool
rb_delete(struct RedBlackNode *restrict *const restrict tree,
	  struct RedBlackNode *const restrict node)
{
	bool restored;

	restored = (node->is_red);

	if (restored)
		rb_replace_red(tree, /* always restorable if node is RED */
			       node);
	else
		restored = rb_replace_black(tree,
					    node);
	/* free node */

	return restored;
}

static inline void
rb_delete_root(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict root)
{
	(void) rb_replace_black(tree,
				root);

	/* free node */
}

void
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const struct Key *const restrict key)
{
	int64_t compare;
	struct RedBlackNode *restrict parent;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent != NULL) {
		compare = key_compare(key,
				      grandparent->key);

		if (compare < 0) {
			parent = grandparent->left;

			if (parent != NULL) {
				compare = key_compare(key,
						      parent->key);


				if (compare < 0) {

				} else if (compare > 0) {

				} else {

				}
			}

		} else if (compare > 0) {
			parent = grandparent->right;

			if (parent != NULL) {
				compare = key_compare(key,
						      parent->key);

				if (compare < 0) {

				} else if (compare > 0) {

				} else {

				}
			}

		} else {
			rb_delete_root(tree,
				       grandparent);
		}
	}
}
