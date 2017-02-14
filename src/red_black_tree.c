#include "red_black_tree.h"			/* Tree, TreeItor */
#include "red_black_tree/red_black_insert.h"	/* red_black_insert */
#include "red_black_tree/red_black_put.h"	/* red_black_put */
#include "red_black_tree/red_black_update.h"	/* red_black_update */
#include "red_black_tree/red_black_add.h"	/* red_black_add */
#include "red_black_tree/red_black_delete.h"	/* red_black_delete */
#include "red_black_tree/red_black_remove.h"	/* red_black_remove */
#include "red_black_tree/red_black_drop.h"	/* red_black_drop */
#include "red_black_tree/red_black_pluck.h"	/* red_black_pluck */
#include "red_black_tree/red_black_find.h"	/* red_black_find */
#include "red_black_tree/red_black_fetch.h"	/* red_black_fetch */
#include "red_black_tree/red_black_replace.h"	/* red_black_replace */
#include "red_black_tree/red_black_exchange.h"	/* red_black_exchange */
#include "red_black_tree/red_black_get.h"	/* red_black_get */
#include "red_black_tree/red_black_set.h"	/* red_black_set */
#include "red_black_tree/red_black_swap.h"	/* red_black_swap */
#include "red_black_tree/red_black_count.h"	/* red_black_count */
#include "red_black_tree/red_black_copy.h"	/* red_black_copy */
#include "red_black_tree/red_black_congruent.h" /* red_black_congruent */
#include "red_black_tree/red_black_print.h"	/* red_black_print */
#include "red_black_tree/red_black_verify.h"	/* red_black_verify */
#include "red_black_tree/red_black_treeify.h"	/* red_black_treeify */


void
red_black_tree_init(RedBlackTree *const restrict tree,
		    const RedBlackComparator comparator)
{
	tree->root	 = NULL;
	tree->comparator = comparator;

	rbnf_init(&tree->factory);
}

bool
red_black_tree_clone(RedBlackTree *const restrict dst_tree,
		     const RedBlackTree *const restrict src_tree)
{
	return rb_tree_clone(dst_tree,
			     src_tree,
			     red_black_count(src_tree->root));
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

	factory_ptr = &dst_tree->factory;

	status = (count == 0);

	if (status) {
		/* finish empty initialization */
		*root_ptr = NULL;
		rbnf_init(factory_ptr);

	} else {
		buffer = rbnf_init_w_nodes(factory_ptr,
					   count);

		status = (buffer != NULL);

		if (status)
			(void) red_black_copy(root_ptr,
					      src_tree->root,
					      buffer);
	}

	return status;
}

void
red_black_tree_destroy(RedBlackTree *const restrict tree)
{
	rbnf_destroy(&tree->factory);
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
				&tree->factory,
				jump_buffer,
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
			     &tree->factory,
			     jump_buffer,
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
				&tree->factory,
				jump_buffer,
				key,
				old_ptr) /* 1, 0 */
	     : RED_BLACK_JUMP_3_STATUS(status); /* 1, 0, -1 */
}

bool
red_black_tree_add(RedBlackTree *const restrict tree,
		   const void *const key)
{
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackNodeFactory *restrict factory_ptr;
	struct RedBlackNode *restrict *restrict root_ptr;
	struct RedBlackNode *restrict node;
	RedBlackComparator comparator;
	int status;

	root_ptr	 = &tree->root;
	comparator       = tree->comparator;
	factory_ptr = &tree->factory;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0)
		return status != RED_BLACK_JUMP_VALUE_3_ERROR;

	node = rbnf_allocate(factory_ptr,
			     jump_buffer);

	node->key = key;

	red_black_add(root_ptr,
		      comparator,
		      jump_buffer,
		      node);

	return true; /* successful addition */
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
				&tree->factory,
				jump_buffer,
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
				    &tree->factory,
				    jump_buffer) /* 1, 0 */
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
				    &tree->factory,
				    jump_buffer) /* 1, 0 */
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
				&tree->factory,
				jump_buffer,
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
				    &tree->factory,
				    jump_buffer,
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
				    &tree->factory,
				    jump_buffer,
				    remove_ptr) /* 1, 0 */
	     : 1; /* will only jump if removed */
}


void
red_black_tree_drop(RedBlackTree *const restrict tree,
		    const void *const key)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_drop(&tree->root,
			       tree->comparator,
			       &tree->factory,
			       jump_buffer,
			       key);
}

void
red_black_tree_drop_min(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_drop_min(&tree->root,
				   &tree->factory,
				   jump_buffer);
}

void
red_black_tree_drop_max(RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;

	if (RED_BLACK_SET_JUMP(jump_buffer) == 0)
		red_black_drop_max(&tree->root,
				   &tree->factory,
				   jump_buffer);
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
				&tree->factory,
				jump_buffer,
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
				    &tree->factory,
				    jump_buffer,
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
				    &tree->factory,
				    jump_buffer,
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
		       const void *const key)
{
	return red_black_replace(tree->root,
				 tree->comparator,
				 key);
}

bool
red_black_tree_replace_min(const RedBlackTree *const restrict tree,
			   const void *const key)
{
	return red_black_replace_min(tree->root,
				     key);
}

bool
red_black_tree_replace_max(const RedBlackTree *const restrict tree,
			   const void *const key)
{
	return red_black_replace_max(tree->root,
				     key);
}


bool
red_black_tree_exchange(const RedBlackTree *const restrict tree,
			const void *const key,
			void **const restrict old_ptr)
{
	return red_black_exchange(tree->root,
				  tree->comparator,
				  key,
				  old_ptr);
}

bool
red_black_tree_exchange_min(const RedBlackTree *const restrict tree,
			    const void *const key,
			    void **const restrict old_ptr)
{
	return red_black_exchange_min(tree->root,
				      key,
				      old_ptr);
}

bool
red_black_tree_exchange_max(const RedBlackTree *const restrict tree,
			    const void *const key,
			    void **const restrict old_ptr)
{
	return red_black_exchange_max(tree->root,
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


bool
red_black_tree_empty(const RedBlackTree *const restrict tree)
{
	return (tree->root == NULL);
}


unsigned int
red_black_tree_count(const RedBlackTree *const restrict tree)
{
	return red_black_count(tree->root);
}


bool
red_black_tree_verify(const RedBlackTree *const restrict tree)
{
	RedBlackJumpBuffer jump_buffer;

	return (RED_BLACK_SET_JUMP(jump_buffer) == 0)
	    && red_black_verify(tree->root,
				tree->comparator,
				jump_buffer);
}


bool
red_black_tree_congruent(const RedBlackTree *const tree1,
			 const RedBlackTree *const tree2)
{
	return (tree1 == tree2)
	    || (   (tree1->comparator == tree2->comparator)
		&& rb_tree_congruent(tree1,
				     tree2));
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
				    jump_buffer);
		/* if returned w/o jump, status is true */

	return status;
}


bool
red_black_tree_similar(const RedBlackTree *const tree1,
		       const RedBlackTree *const tree2)
{
	return (tree1 == tree2)
	    || (   (tree1->comparator == tree2->comparator)
		&& rb_tree_similar(tree1,
				   tree2));
}

bool
rb_tree_similar(const RedBlackTree *const restrict tree1,
		const RedBlackTree *const restrict tree2)
{
	bool status1;
	bool status2;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	RedBlackComparator comparator;
	void *key1;
	void *key2;

	red_black_asc_itor_init(&itor1,
				tree1->root);

	comparator = tree1->comparator;

	red_black_asc_itor_init(&itor2,
				tree2->root);

	while (1) {
		status1 = red_black_itor_next(&itor1,
					      &key1);

		status2 = red_black_itor_next(&itor2,
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
	struct RedBlackNodeFactory *restrict dst_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor;
	volatile int count;
	int status;
	void *key;

	dst_root_ptr    = &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE)
		++count; /* successful insertion */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* RED_BLACK_MALLOC failure */

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_insert(dst_root_ptr,
					  comparator,
					  dst_factory_ptr,
					  jump_buffer,
					  key); /* 1, 0 */

	return count;
}


int
red_black_tree_put_all(RedBlackTree *const restrict dst_tree,
		       const RedBlackTree *const restrict src_tree)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	struct RedBlackNodeFactory *restrict dst_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor;
	volatile int count;
	int status;
	void *key;

	dst_root_ptr	= &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_3_TRUE)
		++count; /* successful insertion */
	else if (status == RED_BLACK_JUMP_VALUE_3_ERROR)
		return -1; /* RED_BLACK_MALLOC failure */

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_put(dst_root_ptr,
				       comparator,
				       dst_factory_ptr,
				       jump_buffer,
				       key); /* 1, 0 */

	return count;
}


bool
red_black_tree_add_all(RedBlackTree *const restrict dst_tree,
		       const RedBlackTree *const restrict src_tree)
{
	return rb_tree_add_all(dst_tree,
			       src_tree,
			       red_black_count(src_tree->root));
}

bool
rb_tree_add_all(RedBlackTree *const restrict dst_tree,
		const RedBlackTree *const restrict src_tree,
		const unsigned int count_src)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor;
	struct RedBlackNode *volatile restrict node;
	bool status;
	void *key;

	status = (count_src == 0);

	if (!status) {
		node = rbnf_allocate_nodes(&dst_tree->factory,
					   count_src);

		status = (node != NULL);

		if (status) {
			dst_root_ptr = &dst_tree->root;
			comparator   = dst_tree->comparator;

			red_black_asc_itor_init(&itor,
						src_tree->root);

			(void) red_black_itor_next(&itor,
						   &key);

			if (RED_BLACK_SET_JUMP(jump_buffer) != 0)
				goto NEXT_KEY;

			while (1) {
				node->key = key;

				red_black_add(dst_root_ptr,
					      comparator,
					      jump_buffer,
					      node);

NEXT_KEY:
				if (!red_black_itor_next(&itor,
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
	struct RedBlackNodeFactory *restrict dst_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor;
	volatile int count;
	int status;
	void *key;

	dst_root_ptr	= &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == RED_BLACK_JUMP_VALUE_2_TRUE)
		++count; /* successful deletion */

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_delete(dst_root_ptr,
					  comparator,
					  dst_factory_ptr,
					  jump_buffer,
					  key); /* 1, 0 */

	return count;
}


void
red_black_tree_drop_all(RedBlackTree *const restrict dst_tree,
			const RedBlackTree *const restrict src_tree)
{
	struct RedBlackNode *restrict *restrict dst_root_ptr;
	struct RedBlackNodeFactory *restrict dst_factory_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor;
	void *key;

	dst_root_ptr	= &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	(void) RED_BLACK_SET_JUMP(jump_buffer);

	while (red_black_itor_next(&itor,
				   &key))
		red_black_drop(dst_root_ptr,
			       comparator,
			       dst_factory_ptr,
			       jump_buffer,
			       key); /* 1, 0 */
}


int
red_black_tree_union(RedBlackTree *const restrict union_tree,
		     const RedBlackTree *const restrict tree1,
		     const RedBlackTree *const restrict tree2)
{
	return rb_tree_union(union_tree,
			     tree1,
			     tree2,
			     red_black_count(tree1->root));
}

int
rb_tree_union(RedBlackTree *const restrict union_tree,
	      const RedBlackTree *const restrict tree1,
	      const RedBlackTree *const restrict tree2,
	      const unsigned int count1)
{
	if (rb_tree_clone(union_tree,
			  tree1,
			  count1)) {
		const int count_inserted = red_black_tree_insert_all(union_tree,
								     tree2);

		if (count_inserted >= 0)
			return count1 + count_inserted;

		rbnf_destroy(&union_tree->factory);
	}

	return -1; /* RED_BLACK_MALLOC failure */
}


int
red_black_tree_intersection(RedBlackTree *const restrict intersection_tree,
			    const RedBlackTree *const restrict tree1,
			    const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict intersection_factory_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;
	int compare;
	int count;

	/* 1. build up list of overlapping keys in ascending order
	 * 2. treeify list */

	red_black_desc_itor_init(&itor1,
				 tree1->root);

	red_black_desc_itor_init(&itor2,
				 tree2->root);

	comparator = tree2->comparator;

	intersection_tree->comparator = comparator;

	intersection_factory_ptr = &intersection_tree->factory;

	rbnf_init(intersection_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(intersection_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}


	/* head  = NULL; */
	count = 0;

	while (1) {
		if (!red_black_itor_next(&itor1,
					 &key1))
			break;

		if (!red_black_itor_next(&itor2,
					 &key2))
			break;


		while (1) {
			compare = comparator(key1,
					     key2);

			if (compare < 0) {
				if (!red_black_itor_next(&itor2,
							 &key2))
					goto TREEIFY;

			} else if (compare > 0) {
				if (!red_black_itor_next(&itor1,
							 &key1))
					goto TREEIFY;

			} else {
				break; /* add overlapping keys */
			}
		}

		node = rbnf_allocate(intersection_factory_ptr,
				     jump_buffer);

		node->key  = key1;
		node->left = head;
		head	   = node;

		++count;
	}

TREEIFY:
	intersection_tree->root = red_black_treeify(head,
						    count,
						    false); /* BLACK */

	return count;
}


int
red_black_tree_difference(RedBlackTree *const restrict difference_tree,
			  const RedBlackTree *const restrict tree1,
			  const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict difference_factory_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict root1;
	struct RedBlackNode *restrict head;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	int count;
	void *key1;
	void *key2;
	int compare;

	/* 1. build up list of keys unique to tree1 in ascending order
	 * 2. treeify list */

	root1 = tree1->root;

	red_black_desc_itor_init(&itor1,
				 root1);

	red_black_desc_itor_init(&itor2,
				 tree2->root);

	comparator = tree2->comparator;

	difference_tree->comparator = comparator;

	difference_factory_ptr = &difference_tree->factory;

	rbnf_init(difference_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(difference_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	/* head  = NULL; */
	count = 0;

	while (red_black_itor_next(&itor2,
				   &key2)) {
		while (1) {
			if (!red_black_itor_next(&itor1,
						 &key1))
				goto TREEIFY;

			compare = comparator(key2,
					     key1);

			if (compare >= 0)
				break;

			node = rbnf_allocate(difference_factory_ptr,
					     jump_buffer);

			node->key  = key1;
			node->left = head;
			head	   = node;

			++count;
		}
	}

	/* tree2 exhausted, add remainder of tree1 */
	while (red_black_itor_next(&itor1,
				   &key1)) {
		node = rbnf_allocate(difference_factory_ptr,
				     jump_buffer);

		node->key  = key1;
		node->left = head;
		head	   = node;

		++count;
	}

TREEIFY:
	difference_tree->root = red_black_treeify(head,
						  count,
						  false); /* BLACK */

	return count;
}


int
red_black_tree_sym_difference(RedBlackTree *const restrict sym_difference_tree,
			      const RedBlackTree *const restrict tree1,
			      const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict sym_difference_factory_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	struct RedBlackItor *restrict greater_itor_ptr;
	struct RedBlackItor *restrict lesser_itor_ptr;
	struct RedBlackItor *restrict tmp_itor_ptr;
	struct RedBlackItor *restrict rem_itor_ptr;
	int count;
	void *rem_key;
	void *greater_key;
	void *lesser_key;
	int compare;

	/* 1. build up list of non-overlapping keys in ascending order
	 * 2. treeify list */

	greater_itor_ptr = &itor1;
	red_black_desc_itor_init(greater_itor_ptr,
				 tree1->root);

	lesser_itor_ptr = &itor2;
	red_black_desc_itor_init(lesser_itor_ptr,
				 tree2->root);

	comparator = tree2->comparator;

	sym_difference_tree->comparator = comparator;

	sym_difference_factory_ptr = &sym_difference_tree->factory;

	rbnf_init(sym_difference_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(sym_difference_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	red_black_tree_init(sym_difference_tree,
			    comparator);

	/* head  = NULL; */
	count = 0;

	while (1) {
		/* set lesser key */
		if (!red_black_itor_next(lesser_itor_ptr,
					 &lesser_key)) {
			rem_itor_ptr = greater_itor_ptr;
			break;
		}

		while (1) {
			/* fetch next key from greater itor */
			if (!red_black_itor_next(greater_itor_ptr,
						 &greater_key)) {
				rem_itor_ptr = lesser_itor_ptr;
				rem_key	     = lesser_key;
				/* lesser_key is unique, ensure it is added */
				goto DO_LISTIFY_REMAINDER;
			}

			compare = comparator(lesser_key,
					     greater_key);

			if (compare == 0)
				break; /* do not add overlapping keys */

			node = rbnf_allocate(sym_difference_factory_ptr,
					     jump_buffer);

			if (compare < 0) {
				/* continue fetching from greater itor */
				node->key = greater_key;

			} else {
				/* itors out of order, need to swap */
				tmp_itor_ptr	 = lesser_itor_ptr;
				lesser_itor_ptr  = greater_itor_ptr;
				greater_itor_ptr = tmp_itor_ptr;

				node->key  = lesser_key;
				lesser_key = greater_key;
			}

			/* push node into list and update count */
			node->left = head;
			head       = node;

			++count;
		}
	}

	while (red_black_itor_next(rem_itor_ptr,
				   &rem_key)) {
DO_LISTIFY_REMAINDER:
		node = rbnf_allocate(sym_difference_factory_ptr,
				     jump_buffer);

		node->key  = rem_key;
		node->left = head;
		head       = node;

		++count;
	}

	/* treeify list (no need to NULL terminate) */
	sym_difference_tree->root = red_black_treeify(head,
						      count,
						      false); /* BLACK */

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
red_black_tree_asc_itor_init(RedBlackTreeItor *const restrict itor,
			     const RedBlackTree *const restrict tree)
{
	red_black_asc_itor_init(itor,
				tree->root);
}


void
red_black_tree_desc_itor_init(RedBlackTreeItor *const restrict itor,
			      const RedBlackTree *const restrict tree)
{
	red_black_desc_itor_init(itor,
				 tree->root);
}


void
red_black_tree_itor_reset(RedBlackTreeItor *const restrict itor,
			  const RedBlackTree *const restrict tree)
{
	red_black_itor_reset(itor,
			     tree->root);
}


bool
red_black_tree_itor_next(RedBlackTreeItor *const restrict itor,
			 void **const restrict key_ptr)
{
	return red_black_itor_next(itor,
				   key_ptr);
}
