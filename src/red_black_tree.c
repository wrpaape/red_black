#include "red_black_tree.h"   /* types */
#include "red_black_insert.h" /* red_black_insert */
#include "red_black_update.h" /* red_black_update */
#include "red_black_delete.h" /* red_black_delete */
#include "red_black_remove.h" /* red_black_remove */
#include "red_black_find.h"   /* red_black_find */
#include "red_black_fetch.h"  /* red_black_fetch */
#include "red_black_count.h"  /* red_black_count */
#include "red_black_print.h"  /* red_black_print */
#include "red_black_verify.h" /* red_black_verify */
#include "red_black_malloc.h" /* RED_BLACK_MALLOC */


void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator)
{
	tree->root	 = NULL;
	tree->comparator = comparator;

	rbnf_init(&tree->node_factory,
		  &node_factory_blueprint);
}

bool
red_black_tree_clone(RedBlackTree *const restrict dst_tree,
		     RedBlackTree *const restrict src_tree)
{
	return rb_tree_clone(dst_tree,
			     src_tree,
			     red_black_tree_count(src_tree));
}


bool
rb_tree_clone(RedBlackTree *const restrict dst_tree,
	      RedBlackTree *const restrict src_tree,
	      const unsigned int count)
{
	/* struct RedBlackNode *restrict *restrict root_ptr; */
	/* bool status; */

	/* root_ptr = &dst_tree->node; */

	/* dst_tree->comparator = src_tree->comparator; */

	/* status = (count == 0); */

	/* if (status) { */
	/* 	/1* finish empty initialization *1/ */
	/* 	*root_ptr = NULL; */
	/* 	rbnf_node_factory_init(&tree->node_factory); */

	/* 	rbnf_init(&dst_tree->node_factory, */
	/* 		  &node_factory_blueprint); */

	/* } else { */
	/* 	*root_ptr = RED_BLACK_MALLOC(sizeof(**root_ptr) * count); */

	/* 	status = (*root_ptr != NULL); */

	/* 	if (status) { */
	/* 	} */
	/* } */

	/* return status; */

	return false;
}

void
red_black_tree_destroy(RedBlackTree *const restrict tree)
{
	rbnf_destroy(&tree->node_factory);
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
				&tree->node_factory,
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
				&tree->node_factory,
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
				&tree->node_factory,
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
				&tree->node_factory,
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
red_black_tree_asc_iterator_init(RedBlackTreeIterator *const restrict iter,
				 const RedBlackTree *const restrict tree)
{
	red_black_asc_iterator_init(iter,
				    tree->root);
}

void
red_black_tree_asc_iterator_set(RedBlackTreeIterator *const restrict iter,
				const RedBlackTree *const restrict tree)
{
	red_black_asc_iterator_set(iter,
				   tree->root);
}

void
red_black_tree_desc_iterator_init(RedBlackTreeIterator *const restrict iter,
				  const RedBlackTree *const restrict tree)
{
	red_black_desc_iterator_init(iter,
				     tree->root);
}

void
red_black_tree_desc_iterator_set(RedBlackTreeIterator *const restrict iter,
				 const RedBlackTree *const restrict tree)
{
	red_black_desc_iterator_set(iter,
				    tree->root);
}

bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iter,
			     void **const restrict key_ptr)
{
	return red_black_iterator_next(iter,
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
