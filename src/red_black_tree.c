#include "red_black_tree.h"
#include "red_black_insert.h"
#include "red_black_delete.h"
#include "red_black_find.h"
#include "red_black_verify.h"
#include "red_black_print.h"
#include "red_black_count.h"


void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator)
{
	tree->root	 = NULL;
	tree->comparator = comparator;

	red_black_allocator_init(&tree->allocator);
}

void
red_black_tree_destroy(RedBlackTree *const restrict tree)
{
	red_black_allocator_destroy(&tree->allocator);
}

int
red_black_tree_insert(RedBlackTree *const restrict tree,
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
				key) /* 1, 0 */
	     : RED_BLACK_JUMP_3_STATUS(status); /* 1, 0, -1 */
}

int
red_black_tree_delete(RedBlackTree *const restrict tree,
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
				key) /* 1, 0 */
	     : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */
}

bool
red_black_tree_find(const RedBlackTree *const restrict tree,
		    const void *const key)
{
	return red_black_find(tree->root,
			      tree->comparator,
			      key);
}

unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree)
{
	return red_black_count(tree->root);
}

void
red_black_tree_iterator_init_asc(RedBlackTreeIterator *const restrict iterator,
				 const RedBlackTree *const restrict tree)
{
	red_black_iterator_init_asc(iterator,
				    tree->root);
}

void
red_black_tree_iterator_init_desc(RedBlackTreeIterator *const restrict iterator,
				  const RedBlackTree *const restrict tree)
{
	red_black_iterator_init_desc(iterator,
				     tree->root);
}

bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iterator,
			     const void **const restrict key_ptr)
{
	return red_black_iterator_next(iterator,
				       key_ptr);
}

bool
red_black_tree_print(const RedBlackTree *const restrict tree,
		     const RedBlackKeySizer key_sizer,
		     const RedBlackKeyPutter key_putter)
{
	return red_black_print(tree->root,
			       key_sizer,
			       key_putter);
}

bool
red_black_tree_verify(const RedBlackTree *const restrict tree)
{
	return red_black_verify(tree->root,
				tree->comparator);
}
