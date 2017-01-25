#include "red_black_tree.h"	 /* types */
#include "red_black_insert.h"	 /* red_black_insert */
#include "red_black_put.h"	 /* red_black_put */
#include "red_black_update.h"	 /* red_black_update */
#include "red_black_add.h"	 /* red_black_add */
#include "red_black_delete.h"	 /* red_black_delete */
#include "red_black_remove.h"	 /* red_black_remove */
#include "red_black_subtract.h"	 /* red_black_subtract */
#include "red_black_pluck.h"	 /* red_black_pluck */
#include "red_black_find.h"	 /* red_black_find */
#include "red_black_fetch.h"	 /* red_black_fetch */
#include "red_black_replace.h"	 /* red_black_replace */
#include "red_black_get.h"	 /* red_black_get */
#include "red_black_set.h"	 /* red_black_set */
#include "red_black_swap.h"	 /* red_black_swap */
#include "red_black_count.h"	 /* red_black_count */
#include "red_black_copy.h"	 /* red_black_verify */
#include "red_black_congruent.h" /* red_black_congruent */
#include "red_black_print.h"	 /* red_black_print */
#include "red_black_verify.h"	 /* red_black_verify */


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
red_black_tree_put(RedBlackTree *const restrict tree,
		   const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_put(&tree->root,
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

bool
red_black_tree_add(RedBlackTree *const restrict tree,
		   const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackNodeFactory *restrict node_factory_ptr;
	struct RedBlackNode *restrict *restrict root_ptr;
	struct RedBlackNode *restrict node;
	RedBlackComparator comparator;
	int status;

	root_ptr	 = &tree->root;
	comparator       = tree->comparator;
	node_factory_ptr = &tree->node_factory;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0)
		return status != RED_BLACK_JUMP_VALUE_3_ERROR;

	node = rbnf_allocate(node_factory_ptr,
			     &jump_buffer);

	node->key = key;

	red_black_add(root_ptr,
		      comparator,
		      &jump_buffer,
		      node);

	return true; /* successful attach */
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
red_black_tree_delete_min(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_delete_min(&tree->root,
				    &tree->node_factory,
				    &jump_buffer) /* 1, 0 */
	     : 1; /* will only jump if deleted */
}

int
red_black_tree_delete_max(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_delete_max(&tree->root,
				    &tree->node_factory,
				    &jump_buffer) /* 1, 0 */
	     : 1; /* will only jump if deleted */
}


int
red_black_tree_remove(RedBlackTree *const restrict tree,
		      const void *const key,
		      void **const restrict remove_ptr)
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
				remove_ptr) /* 1, 0 */
	     : RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */
}

int
red_black_tree_remove_min(RedBlackTree *const restrict tree,
			  void **const restrict remove_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_remove_min(&tree->root,
				    &tree->node_factory,
				    &jump_buffer,
				    remove_ptr) /* 1, 0 */
	     : 1; /* will only jump if removed */
}

int
red_black_tree_remove_max(RedBlackTree *const restrict tree,
			  void **const restrict remove_ptr)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	return (status == 0)
	     ? red_black_remove_max(&tree->root,
				    &tree->node_factory,
				    &jump_buffer,
				    remove_ptr) /* 1, 0 */
	     : 1; /* will only jump if removed */
}


void
red_black_tree_subtract(RedBlackTree *const restrict tree,
			const void *const key)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_subtract(&tree->root,
				   tree->comparator,
				   &tree->node_factory,
				   &jump_buffer,
				   key);
}

void
red_black_tree_subtract_min(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_subtract_min(&tree->root,
				       &tree->node_factory,
				       &jump_buffer);
}

void
red_black_tree_subtract_max(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_subtract_max(&tree->root,
				       &tree->node_factory,
				       &jump_buffer);
}


void *
red_black_tree_pluck(RedBlackTree *const restrict tree,
		     const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	void *volatile plucked_key;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_pluck(&tree->root,
				tree->comparator,
				&tree->node_factory,
				&jump_buffer,
				key,
				(void **) &plucked_key);

	return plucked_key;
}

void *
red_black_tree_pluck_min(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;
	void *volatile plucked_key;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_pluck_min(&tree->root,
				    &tree->node_factory,
				    &jump_buffer,
				    (void **) &plucked_key);

	return plucked_key;
}

void *
red_black_tree_pluck_max(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;
	void *volatile plucked_key;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_pluck_max(&tree->root,
				    &tree->node_factory,
				    &jump_buffer,
				    (void **) &plucked_key);

	return plucked_key;
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
		     void **const restrict fetch_ptr)
{
	return red_black_fetch(tree->root,
			       tree->comparator,
			       key,
			       fetch_ptr);
}

bool
red_black_tree_fetch_min(const RedBlackTree *const restrict tree,
			 void **const restrict fetch_ptr)
{
	return red_black_fetch_min(tree->root,
				   fetch_ptr);
}

bool
red_black_tree_fetch_max(const RedBlackTree *const restrict tree,
			 void **const restrict fetch_ptr)
{
	return red_black_fetch_max(tree->root,
				   fetch_ptr);
}


bool
red_black_tree_replace(const RedBlackTree *const restrict tree,
		       const void *const key,
		       void **const restrict old_ptr)
{
	return red_black_replace(tree->root,
				 tree->comparator,
				 key,
				 old_ptr);
}

bool
red_black_tree_replace_min(const RedBlackTree *const restrict tree,
			   const void *const key,
			   void **const restrict old_ptr)
{
	return red_black_replace_min(tree->root,
				     key,
				     old_ptr);
}

bool
red_black_tree_replace_max(const RedBlackTree *const restrict tree,
			   const void *const key,
			   void **const restrict old_ptr)
{
	return red_black_replace_max(tree->root,
				     key,
				     old_ptr);
}


void *
red_black_tree_get(const RedBlackTree *const restrict tree,
		   const void *const key)
{
	return red_black_get(tree->root,
			     tree->comparator,
			     key);
}

void *
red_black_tree_get_min(const RedBlackTree *const restrict tree)
{
	return red_black_get_min(tree->root);
}

void *
red_black_tree_get_max(const RedBlackTree *const restrict tree)
{
	return red_black_get_max(tree->root);
}


void
red_black_tree_set(const RedBlackTree *const restrict tree,
		   const void *const key)
{
	red_black_set(tree->root,
		      tree->comparator,
		      key);
}

void
red_black_tree_set_min(const RedBlackTree *const restrict tree,
		       const void *const key)
{
	red_black_set_min(tree->root,
			  key);
}
void
red_black_tree_set_max(const RedBlackTree *const restrict tree,
		       const void *const key)
{
	red_black_set_max(tree->root,
			  key);
}


void *
red_black_tree_swap(const RedBlackTree *const restrict tree,
		    const void *const key)
{
	return red_black_swap(tree->root,
			      tree->comparator,
			      key);
}

void *
red_black_tree_swap_min(const RedBlackTree *const restrict tree,
			const void *const key)
{
	return red_black_swap_min(tree->root,
				  key);
}

void *
red_black_tree_swap_max(const RedBlackTree *const restrict tree,
			const void *const key)
{
	return red_black_swap_max(tree->root,
				  key);
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
red_black_tree_congruent(const RedBlackTree *const tree1,
			 const RedBlackTree *const tree2)
{
	bool status;

	status = (tree1 == tree2);

	if (!status) {
		status = (tree1->comparator == tree2->comparator);

		if (status)
			status = rb_tree_congruent(tree1,
						   tree2);
	}

	return status;

}

bool
rb_tree_congruent(const RedBlackTree *const restrict tree1,
		  const RedBlackTree *const restrict tree2)
{
	bool status;
	RedBlackJumpBuffer jump_buffer;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	if (status)
		red_black_congruent(tree1->root,
				    tree2->root,
				    tree1->comparator,
				    &jump_buffer);
		/* if returned w/o jump, status is true */

	return status;
}


bool
red_black_tree_similar(const RedBlackTree *const tree1,
		       const RedBlackTree *const tree2)
{
	bool status;

	status = (tree1 == tree2);

	if (!status) {
		status = (tree1->comparator == tree2->comparator);

		if (status)
			status = rb_tree_similar(tree1,
						 tree2);
	}

	return status;
}

bool
rb_tree_similar(const RedBlackTree *const restrict tree1,
		const RedBlackTree *const restrict tree2)
{
	bool status1;
	bool status2;
	struct RedBlackIterator iter1;
	struct RedBlackIterator iter2;
	RedBlackComparator comparator;
	void *key1;
	void *key2;

	red_black_asc_iterator_init(&iter1,
				    tree1->root);

	comparator = tree1->comparator;

	red_black_asc_iterator_init(&iter2,
				    tree2->root);

	while (1) {
		status1 = red_black_iterator_next(&iter1,
						  &key1);

		status2 = red_black_iterator_next(&iter2,
						  &key2);

		if (status1 != status2)
			return false;

		if (!status1)
			return true;

		if (comparator(key1,
			       key2) != 0)
			return false;
	}
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
	volatile int count;
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


/* int */
/* red_black_tree_update_all(RedBlackTree *const restrict dst_tree, */
/* 			  const RedBlackTree *const restrict src_tree) */
/* { */
/* 	struct RedBlackNode *restrict *restrict dst_root_ptr; */
/* 	struct RedBlackNodeFactory *restrict dst_node_factory_ptr; */
/* 	RedBlackComparator comparator; */
/* 	RedBlackJumpBuffer jump_buffer; */
/* 	struct RedBlackIterator iter; */
/* 	volatile int count; */
/* 	int status; */
/* 	void *key; */
/* 	void *old_key; */

/* 	dst_root_ptr	     = &dst_tree->root; */
/* 	comparator	     = dst_tree->comparator; */
/* 	dst_node_factory_ptr = &dst_tree->node_factory; */

/* 	red_black_asc_iterator_init(&iter, */
/* 				    src_tree->root); */

/* 	count = 0; */

/* 	status = RED_BLACK_SET_JUMP(jump_buffer); */

/* 	if (status == RED_BLACK_JUMP_VALUE_3_TRUE) */
/* 		++count; /1* successful insertion *1/ */
/* 	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR) */
/* 		return -1; /1* RED_BLACK_MALLOC failure *1/ */

/* 	while (red_black_iterator_next(&iter, */
/* 				       &key)) */
/* 		count += red_black_update(dst_root_ptr, */
/* 					  comparator, */
/* 					  dst_node_factory_ptr, */
/* 					  &jump_buffer, */
/* 					  key, */
/* 					  &old_key); /1* 1, 0 *1/ */

/* 	return count; */
/* } */


bool
red_black_tree_add_all(RedBlackTree *const restrict dst_tree,
		       const RedBlackTree *const restrict src_tree)
{
	return rb_tree_add_all(dst_tree,
			       src_tree,
			       red_black_tree_count(src_tree));
}

bool
rb_tree_add_all(RedBlackTree *const restrict dst_tree,
		const RedBlackTree *const restrict src_tree,
		const unsigned int count_src)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackIterator iter;
	struct RedBlackNode *volatile restrict node;
	bool status;
	void *key;

	status = (count_src == 0);

	if (!status) {
		node = rbnf_allocate_nodes(&dst_tree->node_factory,
					   count_src);

		status = (node != NULL);

		if (status) {
			dst_root_ptr = &dst_tree->root;
			comparator   = dst_tree->comparator;

			red_black_asc_iterator_init(&iter,
						    src_tree->root);

			(void) red_black_iterator_next(&iter,
						       &key);

			if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
				goto NEXT_KEY;

			while (1) {
				node->key = key;

				red_black_add(dst_root_ptr,
					      comparator,
					      &jump_buffer,
					      node);
NEXT_KEY:
				if (!red_black_iterator_next(&iter,
							     &key))
					break;

				++node;
			}
		}
	}

	return status;
}


int
red_black_tree_delete_all(RedBlackTree *const restrict dst_tree,
			  const RedBlackTree *const restrict src_tree)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	struct RedBlackNodeFactory *restrict dst_node_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackIterator iter;
	volatile int count;
	int status;
	void *key;

	dst_root_ptr	     = &dst_tree->root;
	comparator	     = dst_tree->comparator;
	dst_node_factory_ptr = &dst_tree->node_factory;

	red_black_asc_iterator_init(&iter,
				    src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_2_TRUE)
		++count; /* successful deletion */

	while (red_black_iterator_next(&iter,
				       &key))
		count += red_black_delete(dst_root_ptr,
					  comparator,
					  dst_node_factory_ptr,
					  &jump_buffer,
					  key); /* 1, 0 */

	return count;
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
	     ? (count1 + red_black_tree_insert_all(union_tree,
						   tree2))
	     : -1; /* RED_BLACK_MALLOC failure */
}


int
red_black_tree_intersect(RedBlackTree *const restrict intersect_tree,
			 const RedBlackTree *const restrict tree1,
			 const RedBlackTree *const restrict tree2)
{

	struct RedBlackNode *restrict *restrict intersect_root_ptr;
	struct RedBlackNodeFactory *restrict intersect_node_factory_ptr;
	const struct RedBlackNode *restrict root1;
	struct RedBlackNode *restrict node;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackIterator iter2;
	volatile int count;
	void *key;

	root1      = tree1->root;
	comparator = tree1->comparator;

	intersect_root_ptr         = &intersect_tree->root;
	intersect_node_factory_ptr = &intersect_tree->node_factory;

	red_black_tree_init(intersect_tree,
			    comparator);

	red_black_asc_iterator_init(&iter2,
				    tree2->root);

	count = 0;

	if (RED_BLACK_SET_JUMP(jump_buffer) == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* RED_BLACK_MALLOC failure */

	while (red_black_iterator_next(&iter2,
				       &key))
		if (red_black_find(root1,
				   comparator,
				   key)) {

			++count;

			node = rbnf_allocate(intersect_node_factory_ptr,
					     &jump_buffer);

			node->key = key;

			red_black_add(intersect_root_ptr,
				      comparator,
				      &jump_buffer,
				      node);
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
red_black_tree_desc_iterator_init(RedBlackTreeIterator *const restrict iter,
				  const RedBlackTree *const restrict tree)
{
	red_black_desc_iterator_init(iter,
				     tree->root);
}


void
red_black_tree_iterator_reset(RedBlackTreeIterator *const restrict iter,
			      const RedBlackTree *const restrict tree)
{
	red_black_iterator_reset(iter,
				 tree->root);
}


bool
red_black_tree_iterator_next(RedBlackTreeIterator *const restrict iter,
			     void **const restrict key_ptr)
{
	return red_black_iterator_next(iter,
				       key_ptr);
}
