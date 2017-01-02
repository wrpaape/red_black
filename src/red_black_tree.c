#include "red_black_tree.h"
#include "red_black_insert.h"
#include "red_black_delete.h"
#include "red_black_find.h"
#include "red_black_verify.h"
#include "red_black_print.h"


void
red_black_tree_init(struct RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator)
{
	tree->root	 = NULL;
	tree->comparator = comparator;

	red_black_allocator_init(&tree->allocator);
}

int
red_black_tree_insert(struct RedBlackTree *const restrict tree,
		      const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_insert(&tree->root,
				tree->comparator,
				&tree->allocator,
				&jump_buffer,
				key)
	     : RED_BLACK_JUMP_3_STATUS(status); /* 1, 0, -1 */
}

bool
red_black_tree_delete(struct RedBlackTree *const restrict tree,
		      const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* return (status == 0) */
	/*      ? red_black_delete(&tree->root, */
	/* 			tree->comparator, */
	/* 			&tree->allocator, */
	/* 			&jump_buffer, */
	/* 			key) */
	/*      : ((bool) RED_BLACK_JUMP_2_STATUS(status)); /1* 1, 0 *1/ */

	bool retval;

	if (status == 0) {
		retval = red_black_delete(&tree->root,
					  tree->comparator,
					  &tree->allocator,
					  &jump_buffer,
					  key);
		DEBUG("RETURNING FROM red_black_tree_delete\n");

	} else {
		retval = (bool) RED_BLACK_JUMP_2_STATUS(status);
		DEBUG("JUMPING FROM red_black_tree_delete\n");
	}

	return retval;
}

bool
red_black_tree_find(struct RedBlackTree *const restrict tree,
		    const void *const key)
{
	return red_black_find(tree->root,
			      tree->comparator,
			      key);
}

bool
red_black_tree_verify(struct RedBlackTree *const restrict tree)
{
	return red_black_verify(tree->root,
				tree->comparator);
}

bool
red_black_tree_print(struct RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter)
{
	return red_black_print(tree->root,
			       key_sizer,
			       key_putter);
}

void
red_black_tree_destroy(struct RedBlackTree *const restrict tree)
{
	red_black_allocator_destroy(&tree->allocator);
}
