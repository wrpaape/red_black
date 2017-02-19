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

	root_ptr    = &tree->root;
	comparator  = tree->comparator;
	factory_ptr = &tree->factory;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status != 0)
		return (status > 0); /* true if no error */

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


static inline bool
rbt_congruent(const struct RedBlackNode *const restrict root1,
	      const struct RedBlackNode *const restrict root2,
	      const RedBlackComparator comparator)
{
	bool status;
	RedBlackJumpBuffer jump_buffer;

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0);

	if (status)
		red_black_congruent(root1,
				    root2,
				    comparator,
				    jump_buffer);
		/* if returned w/o jump, status is true */

	return status;
}

bool
red_black_tree_congruent(const RedBlackTree *const tree1,
			 const RedBlackTree *const tree2)
{
	if (tree1 == tree2)
		return true;

	const RedBlackComparator comparator = tree1->comparator;

	return (   (comparator == tree2->comparator)
		&& rbt_congruent(tree1->root,
				 tree2->root,
				 comparator));
}


static inline bool
rbt_similar(const struct RedBlackNode *const restrict root1,
	    const struct RedBlackNode *const restrict root2,
	    const RedBlackComparator comparator)
{
	bool status1;
	bool status2;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;

	red_black_asc_itor_init(&itor1,
				root1);

	red_black_asc_itor_init(&itor2,
				root2);

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

bool
red_black_tree_similar(const RedBlackTree *const tree1,
		       const RedBlackTree *const tree2)
{
	if (tree1 == tree2)
		return true;

	const RedBlackComparator comparator = tree1->comparator;

	return (   (comparator == tree2->comparator)
		&& rbt_similar(tree1->root,
			       tree2->root,
			       comparator));
}


static inline bool
rbt_intersect(const struct RedBlackNode *const restrict root1,
	      const struct RedBlackNode *const restrict root2,
	      const RedBlackComparator comparator)
{
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;
	int compare;

	red_black_asc_itor_init(&itor1,
				root1);

	if (!red_black_itor_next(&itor1,
				 &key1))
		return false; /* tree1 empty */

	red_black_asc_itor_init(&itor2,
				root2);

	if (!red_black_itor_next(&itor2,
				 &key2))
		return false; /* tree2 empty */

	while (1) {
		compare = comparator(key1,
				     key2);

		if (compare < 0) {
			/* need to advance itor1 */
			if (!red_black_itor_next(&itor1,
						 &key1))
				return false; /* tree1 exhausted */

		} else if (compare > 0) {
			/* need to advance itor2 */
			if (!red_black_itor_next(&itor2,
						 &key2))
				return false; /* tree2 exhausted */

		} else {
			return true; /* intersection found */
		}
	}
}

bool
red_black_tree_intersect(const RedBlackTree *const tree1,
			 const RedBlackTree *const tree2)
{
	if (tree1 == tree2)
		return true;

	const RedBlackComparator comparator = tree1->comparator;

	return (   (comparator == tree2->comparator)
		&& rbt_intersect(tree1->root,
				 tree2->root,
				 comparator));
}


static inline bool
rbt_subset(const struct RedBlackNode *const restrict root1,
	   const struct RedBlackNode *const restrict root2,
	   const RedBlackComparator comparator)
{
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;
	int compare;

	/* emit keys from itor1 while they are less than itor2
	 *
	 * if key1 == key2, advance both itors
	 *
	 * if key1 > key2, tree1 is missing key2 -> not a subset */

	red_black_asc_itor_init(&itor2,
				root2);

	red_black_asc_itor_init(&itor1,
				root1);

	while (1) {
		/* set key2 */
		if (!red_black_itor_next(&itor2,
					 &key2))
			return true; /* all of tree2 contained by tree1 */

		while (1) {
			if (!red_black_itor_next(&itor1,
						 &key1))
				return false; /* tree1 exhausted before tree2 */

			compare = comparator(key2,
					     key1);

			if (compare == 0)
				break; /* advance both itors */

			if (compare < 0)
				return false; /* tree2 has keys < tree1 keys */
		}
	}
}

bool
red_black_tree_subset(const RedBlackTree *const tree1,
		      const RedBlackTree *const tree2)
{
	if (tree1 == tree2)
		return true;

	const RedBlackComparator comparator = tree1->comparator;

	return (   (comparator == tree2->comparator)
		&& rbt_subset(tree1->root,
			      tree2->root,
			      comparator));
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
	volatile unsigned int count;
	int status;
	void *key;

	dst_root_ptr    = &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status < 0)
		return -1; /* RED_BLACK_MALLOC failure */

	/* increment count if insertion was made */
	count += (status == RED_BLACK_JUMP_VALUE_3_TRUE);

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_insert(dst_root_ptr,
					  comparator,
					  dst_factory_ptr,
					  jump_buffer,
					  key); /* 1, 0 */

	return (int) count;
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
	volatile unsigned int count;
	int status;
	void *key;

	dst_root_ptr	= &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status < 0)
		return -1; /* RED_BLACK_MALLOC failure */

	/* increment count if insertion was made */
	count += (status == RED_BLACK_JUMP_VALUE_3_TRUE);

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_put(dst_root_ptr,
				       comparator,
				       dst_factory_ptr,
				       jump_buffer,
				       key); /* 1, 0 */

	return (int) count;
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
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict nodes;
	struct RedBlackNode *volatile restrict buffer;
	bool status;
	void *key;

	status = (count_src == 0);

	if (!status) {
		nodes = rbnf_allocate_nodes(&dst_tree->factory,
					    count_src);

		status = (nodes != NULL);

		if (status) {
			dst_root_ptr = &dst_tree->root;
			comparator   = dst_tree->comparator;

			red_black_asc_itor_init(&itor,
						src_tree->root);

			buffer = nodes;

			(void) RED_BLACK_SET_JUMP(jump_buffer);

			while (red_black_itor_next(&itor,
						   &key)) {
				node = buffer++;

				node->key = key;

				red_black_add(dst_root_ptr,
					      comparator,
					      jump_buffer,
					      node);
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
	volatile unsigned int count;
	int status;
	void *key;

	dst_root_ptr	= &dst_tree->root;
	comparator	= dst_tree->comparator;
	dst_factory_ptr = &dst_tree->factory;

	red_black_asc_itor_init(&itor,
				src_tree->root);

	count = 0;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	/* add 1 if jumped and deletion was made */
	count += RED_BLACK_JUMP_2_STATUS(status); /* 1, 0 */

	while (red_black_itor_next(&itor,
				   &key))
		count += red_black_delete(dst_root_ptr,
					  comparator,
					  dst_factory_ptr,
					  jump_buffer,
					  key); /* 1, 0 */

	return (int) count;
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
	struct RedBlackNodeFactory *restrict union_factory_ptr;
	struct RedBlackNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	struct RedBlackItor *restrict lesser_itor_ptr;
	struct RedBlackItor *restrict greater_itor_ptr;
	struct RedBlackItor *restrict tmp_itor_ptr;
	struct RedBlackItor *restrict rem_itor_ptr;
	void *rem_key;
	void *lesser_key;
	void *greater_key;
	int compare;
	unsigned int count;

	/* 1. build up list of all keys (no duplicates) in ascending order
	 * 2. treeify list */

	lesser_itor_ptr = &itor1;
	red_black_asc_itor_init(lesser_itor_ptr,
				tree1->root);

	greater_itor_ptr = &itor2;
	red_black_asc_itor_init(greater_itor_ptr,
				tree2->root);

	comparator = tree2->comparator;

	union_tree->comparator = comparator;

	union_factory_ptr = &union_tree->factory;

	rbnf_init(union_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(union_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &union_factory_ptr->buffer;

	end_ptr = &head;
	count   = 0;

	while (1) {
		/* set greater key */
		if (!red_black_itor_next(greater_itor_ptr,
					 &greater_key)) {
			rem_itor_ptr = lesser_itor_ptr;
			break;
		}

		while (1) {
			/* fetch next key from lesser itor */
			if (!red_black_itor_next(lesser_itor_ptr,
						 &lesser_key)) {
				rem_itor_ptr = greater_itor_ptr;
				rem_key	     = greater_key;
				/* greater_key is unique, ensure it is added */
				goto DO_LISTIFY_REMAINDER;
			}

			compare = comparator(lesser_key,
					     greater_key);

			node = rbnfb_allocate(factory_buffer_ptr,
					      jump_buffer);

			/* append node to list and update count */
			*end_ptr = node;
			end_ptr  = &node->right;

			++count;

			if (compare < 0) {
				/* continue fetching from lesser itor */
				node->key = lesser_key;

			} else {
				node->key = greater_key;

				if (compare == 0)
					break; /* advance both itors */

				/* itors out of order, need to swap */
				greater_key = lesser_key;

				tmp_itor_ptr	 = greater_itor_ptr;
				greater_itor_ptr = lesser_itor_ptr;
				lesser_itor_ptr  = tmp_itor_ptr;
			}
		}
	}

	while (red_black_itor_next(rem_itor_ptr,
				   &rem_key)) {
DO_LISTIFY_REMAINDER:
		node = rbnfb_allocate(factory_buffer_ptr,
				      jump_buffer);

		*end_ptr   = node;
		end_ptr    = &node->right;
		node->key  = rem_key;

		++count;
	}

	/* treeify list (no need to NULL terminate) */
	union_tree->root = red_black_treeify(head,
					     count);

	return (int) count;
}


int
red_black_tree_intersection(RedBlackTree *const restrict intersection_tree,
			    const RedBlackTree *const restrict tree1,
			    const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict intersection_factory_ptr;
	struct RedBlackNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;
	int compare;
	unsigned int count;

	/* 1. build up list of overlapping keys in ascending order
	 * 2. treeify list */

	red_black_asc_itor_init(&itor1,
				tree1->root);

	red_black_asc_itor_init(&itor2,
				tree2->root);

	comparator = tree2->comparator;

	intersection_tree->comparator = comparator;

	intersection_factory_ptr = &intersection_tree->factory;

	rbnf_init(intersection_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(intersection_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &intersection_factory_ptr->buffer;

	end_ptr = &head;
	count   = 0;

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
				/* need to advance itor1 */
				if (!red_black_itor_next(&itor1,
							 &key1))
					goto TREEIFY;

			} else if (compare > 0) {
				/* need to advance itor2 */
				if (!red_black_itor_next(&itor2,
							 &key2))
					goto TREEIFY;

			} else {
				break; /* add overlapping keys */
			}
		}

		node = rbnfb_allocate(factory_buffer_ptr,
				      jump_buffer);

		*end_ptr  = node;
		end_ptr   = &node->right;
		node->key = key1;

		++count;
	}

TREEIFY:
	intersection_tree->root = red_black_treeify(head,
						    count);

	return (int) count;
}


int
red_black_tree_difference(RedBlackTree *const restrict difference_tree,
			  const RedBlackTree *const restrict tree1,
			  const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict difference_factory_ptr;
	struct RedBlackNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict root1;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	void *key1;
	void *key2;
	int compare;
	unsigned int count;

	/* 1. build up list of keys unique to tree1 in ascending order
	 * 2. treeify list */

	root1 = tree1->root;

	red_black_asc_itor_init(&itor1,
				root1);

	red_black_asc_itor_init(&itor2,
				tree2->root);

	comparator = tree2->comparator;

	difference_tree->comparator = comparator;

	difference_factory_ptr = &difference_tree->factory;

	rbnf_init(difference_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(difference_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &difference_factory_ptr->buffer;

	end_ptr = &head;
	count   = 0;

	while (red_black_itor_next(&itor2,
				   &key2)) {
		while (1) {
			if (!red_black_itor_next(&itor1,
						 &key1))
				goto TREEIFY;

			compare = comparator(key1,
					     key2);

			if (compare >= 0)
				break;

			node = rbnfb_allocate(factory_buffer_ptr,
					      jump_buffer);

			*end_ptr  = node;
			end_ptr   = &node->right;
			node->key = key1;

			++count;
		}
	}

	/* tree2 exhausted, add remainder of tree1 */
	while (red_black_itor_next(&itor1,
				   &key1)) {
		node = rbnfb_allocate(factory_buffer_ptr,
				      jump_buffer);

		*end_ptr  = node;
		end_ptr   = &node->right;
		node->key = key1;

		++count;
	}

TREEIFY:
	difference_tree->root = red_black_treeify(head,
						  count);

	return (int) count;
}


int
red_black_tree_sym_difference(RedBlackTree *const restrict sym_difference_tree,
			      const RedBlackTree *const restrict tree1,
			      const RedBlackTree *const restrict tree2)
{
	struct RedBlackNodeFactory *restrict sym_difference_factory_ptr;
	struct RedBlackNodeFactoryBuffer *restrict factory_buffer_ptr;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	struct RedBlackNode *restrict *restrict end_ptr;
	RedBlackComparator comparator;
	RedBlackJumpBuffer jump_buffer;
	struct RedBlackItor itor1;
	struct RedBlackItor itor2;
	struct RedBlackItor *restrict lesser_itor_ptr;
	struct RedBlackItor *restrict greater_itor_ptr;
	struct RedBlackItor *restrict tmp_itor_ptr;
	struct RedBlackItor *restrict rem_itor_ptr;
	unsigned int count;
	void *rem_key;
	void *lesser_key;
	void *greater_key;
	int compare;

	/* 1. build up list of non-overlapping keys in ascending order
	 * 2. treeify list */

	lesser_itor_ptr = &itor1;
	red_black_asc_itor_init(lesser_itor_ptr,
				tree1->root);

	greater_itor_ptr = &itor2;
	red_black_asc_itor_init(greater_itor_ptr,
				tree2->root);

	comparator = tree2->comparator;

	sym_difference_tree->comparator = comparator;

	sym_difference_factory_ptr = &sym_difference_tree->factory;

	rbnf_init(sym_difference_factory_ptr);

	if (RED_BLACK_SET_JUMP(jump_buffer) != 0) {
		rbnf_destroy(sym_difference_factory_ptr);
		return -1; /* RED_BLACK_MALLOC failure */
	}

	factory_buffer_ptr = &sym_difference_factory_ptr->buffer;

	end_ptr = &head;
	count   = 0;

	while (1) {
		/* set greater key */
		if (!red_black_itor_next(greater_itor_ptr,
					 &greater_key)) {
			rem_itor_ptr = lesser_itor_ptr;
			break;
		}

		while (1) {
			/* fetch next key from lesser itor */
			if (!red_black_itor_next(lesser_itor_ptr,
						 &lesser_key)) {
				rem_itor_ptr = greater_itor_ptr;
				rem_key	     = greater_key;
				/* greater_key is unique, ensure it is added */
				goto DO_LISTIFY_REMAINDER;
			}

			compare = comparator(lesser_key,
					     greater_key);

			if (compare == 0)
				break; /* do not add overlapping keys */

			node = rbnfb_allocate(factory_buffer_ptr,
					      jump_buffer);

			/* append node to list and update count */
			*end_ptr = node;
			end_ptr  = &node->right;

			++count;

			if (compare < 0) {
				/* continue fetching from lesser itor */
				node->key = lesser_key;

			} else {
				/* itors out of order, need to swap */
				node->key   = greater_key;
				greater_key = lesser_key;

				tmp_itor_ptr	 = greater_itor_ptr;
				greater_itor_ptr = lesser_itor_ptr;
				lesser_itor_ptr  = tmp_itor_ptr;
			}
		}
	}

	while (red_black_itor_next(rem_itor_ptr,
				   &rem_key)) {
DO_LISTIFY_REMAINDER:
		node = rbnfb_allocate(factory_buffer_ptr,
				      jump_buffer);

		*end_ptr   = node;
		end_ptr    = &node->right;
		node->key  = rem_key;

		++count;
	}

	/* treeify list (no need to NULL terminate) */
	sym_difference_tree->root = red_black_treeify(head,
						      count);

	return (int) count;
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
