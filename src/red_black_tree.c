#include "red_black_tree.h"	/* types */
#include "red_black_insert.h"	/* red_black_insert */
#include "red_black_update.h"	/* red_black_update */
#include "red_black_delete.h"	/* red_black_delete */
#include "red_black_remove.h"	/* red_black_remove */
#include "red_black_find.h"	/* red_black_find */
#include "red_black_fetch.h"	/* red_black_fetch */
#include "red_black_count.h"	/* red_black_count */
#include "red_black_print.h"	/* red_black_print */
#include "red_black_verify.h"	/* red_black_verify */


void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator)
{
	tree->root	 = NULL;
	tree->comparator = comparator;

	node_allocator_init(&tree->allocator);
}

void
red_black_tree_destroy(RedBlackTree *const restrict tree)
{
	rba_destroy(&tree->allocator);
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
red_black_tree_update(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict old_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_update(&tree->root,
				tree->comparator,
				&tree->allocator,
				&jump_buffer,
				key,
				old_ptr) /* 1, 0 */
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

int
red_black_tree_remove(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict key_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_remove(&tree->root,
				tree->comparator,
				&tree->allocator,
				&jump_buffer,
				key,
				key_ptr) /* 1, 0 */
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

bool
red_black_tree_fetch(const RedBlackTree *const restrict tree,
		     const void *const key,
		     void **const restrict key_ptr)
{
	return red_black_fetch(tree->root,
			       tree->comparator,
			       key,
			       key_ptr);
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

void
red_black_tree_iterator_reset_asc(RedBlackTreeIterator *const restrict iterator,
				  const RedBlackTree *const restrict tree)
{
	red_black_iterator_reset_asc(iterator,
				     tree->root);
}

void
red_black_tree_iterator_reset_desc(RedBlackTreeIterator *const restrict iterator,
				   const RedBlackTree *const restrict tree)
{
	red_black_iterator_reset_desc(iterator,
				      tree->root);
}

bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iterator,
			     void **const restrict key_ptr)
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
	bool status;
	RedBlackJumpBuffer jump_buffer;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	if (status)
		status = red_black_verify(tree->root,
					  tree->comparator,
					  &jump_buffer);

	return status;
}
