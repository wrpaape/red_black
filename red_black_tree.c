#include "red_black_tree.h"
#include "red_black_insert.h"
#include "red_black_delete.h"
#include "red_black_find.h"


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
	     : RED_BLACK_JUMP_STATUS(status);
}

int
red_black_tree_delete(struct RedBlackTree *const restrict tree,
		      const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_delete(&tree->root,
				tree->comparator,
				&tree->allocator,
				&jump_buffer,
				key)
	     : RED_BLACK_JUMP_STATUS(status);
}

int
red_black_tree_find(struct RedBlackTree *const restrict tree,
		    const void *const key)
{
	return red_black_find(tree->root,
			      tree->comparator,
			      key);
}

void
red_black_tree_destroy(struct RedBlackTree *const restrict tree)
{
	red_black_allocator_destroy(&tree->allocator);
}
