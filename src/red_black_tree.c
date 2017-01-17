#include "red_black_tree.h"   /* types */
#include "red_black_insert.h" /* red_black_insert */
#include "red_black_update.h" /* red_black_update */
#include "red_black_append.h" /* red_black_append */
#include "red_black_delete.h" /* red_black_delete */
#include "red_black_remove.h" /* red_black_remove */
#include "red_black_find.h"   /* red_black_find */
#include "red_black_fetch.h"  /* red_black_fetch */
#include "red_black_count.h"  /* red_black_count */
#include "red_black_copy.h"   /* red_black_verify */
#include "red_black_equal.h"  /* red_black_equal */
#include "red_black_print.h"  /* red_black_print */
#include "red_black_verify.h" /* red_black_verify */


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
		     const RedBlackTree *const restrict src_tree)
{
	return rb_tree_clone(dst_tree,
			     src_tree,
			     red_black_tree_count(src_tree));
}


bool
rb_tree_clone(RedBlackTree *const restrict dst_tree,
	      const RedBlackTree *const restrict src_tree,
	      const unsigned int count)
{
	struct RedBlackNode *restrict buffer;
	struct RedBlackNode *restrict *restrict root_ptr;
	struct RedBlackNodeFactory *restrict factory_ptr;
	bool status;

	root_ptr = &dst_tree->root;

	dst_tree->comparator = src_tree->comparator;

	factory_ptr = &dst_tree->node_factory;

	status = (count == 0);

	if (status) {
		/* finish empty initialization */
		*root_ptr = NULL;
		rbnf_init(factory_ptr,
			  &node_factory_blueprint);

	} else {
		buffer = rbnf_init_w_nodes(factory_ptr,
					   &node_factory_blueprint,
					   count);

		status = (buffer != NULL);

		if (status)
			(void) red_black_copy_nodes(root_ptr,
						    src_tree->root,
						    buffer);
	}

	return status;
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

bool
red_black_tree_equal(const RedBlackTree *const tree1,
		     const RedBlackTree *const tree2)
{
	bool status;
	RedBlackJumpBuffer jump_buffer;
	RedBlackComparator comparator;

	status = (tree1 == tree2);

	if (!status) {
		comparator = tree1->comparator;

		status = (comparator == tree2->comparator);

		if (status) {
			status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

			if (status) {
				red_black_equal(tree1->root,
						tree2->root,
						comparator,
						&jump_buffer);

				return true; /* returned w/ no jump */
			}
		}
	}

	return status;

}


int
red_black_tree_insert_all(RedBlackTree *const restrict dst_tree,
			  const RedBlackTree *const restrict src_tree)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	struct RedBlackNodeFactory *restrict dst_node_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackIterator iter;
	int count;
	int status;
	void *key;

	dst_root_ptr	     = &dst_tree->root;
	comparator	     = dst_tree->comparator;
	dst_node_factory_ptr = &dst_tree->node_factory;


	red_black_asc_iterator_init(&iter,
				    src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE)
		++count; /* successful insertion */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* RED_BLACK_MALLOC failure */

	while (red_black_iterator_next(&iter,
				       &key))
		count += red_black_insert(dst_root_ptr,
					  comparator,
					  dst_node_factory_ptr,
					  &jump_buffer,
					  key); /* 1, 0 */

	return count;
}


int
red_black_tree_delete_all(RedBlackTree *const restrict dst_tree,
			  const RedBlackTree *const restrict src_tree)
{
	return -1;
}


int
red_black_tree_union(RedBlackTree *const restrict union_tree,
		     const RedBlackTree *const restrict tree1,
		     const RedBlackTree *const restrict tree2)
{
	return rb_tree_union(union_tree,
			     tree1,
			     tree2,
			     red_black_tree_count(tree1));
}

int
rb_tree_union(RedBlackTree *const restrict union_tree,
	      const RedBlackTree *const restrict tree1,
	      const RedBlackTree *const restrict tree2,
	      const unsigned int count1)
{
	return rb_tree_clone(union_tree,
			     tree1,
			     count1)
	     ? red_black_tree_insert_all(union_tree,
					 tree2)
	     : -1; /* RED_BLACK_MALLOC failure */
}


int
red_black_tree_intersection(RedBlackTree *const restrict intersection_tree,
			    const RedBlackTree *const restrict tree1,
			    const RedBlackTree *const restrict tree2)
{

	struct RedBlackNode *restrict *restrict intersection_root_ptr;
	struct RedBlackNodeFactory *restrict intersection_node_factory_ptr;
	const struct RedBlackNode *restrict root1;
	struct RedBlackNode *restrict node;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackIterator iter2;
	int count;
	void *key;

	root1      = tree1->root;
	comparator = tree1->comparator;

	intersection_root_ptr         = &intersection_tree->root;
	intersection_node_factory_ptr = &intersection_tree->node_factory;

	red_black_tree_init(intersection_tree,
			    comparator);

	red_black_asc_iterator_init(&iter2,
				    tree2->root);

	count = 0;

	if (RED_BLACK_SET_JUMP(jump_buffer) == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* RED_BLACK_MALLOC failure */

	while (red_black_iterator_next(&iter2,
				       &key)) {

		if (red_black_find(root1,
				   comparator,
				   key)) {

			++count;

			node = rbnf_allocate(intersection_node_factory_ptr,
					     &jump_buffer);

			node->key = key;

			red_black_append(intersection_root_ptr,
					 comparator,
					 &jump_buffer,
					 node);
		}
	}

	return count;
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
