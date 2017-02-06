#include "red_black_itor.h"         /* RedBlackItor */
#include "red_black_link_offset.h"  /* asc|desc_link_offset, stddef */

#if 0
#include "red_black_itor_restore.h" /* asc|desc_itor_restore */


#else
static void
red_black_asc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			   struct RedBlackNodeFactory *const restrict factory)
{
}
static void
red_black_desc_itor_restore(struct RedBlackItorCursor *const restrict cursor,
			    struct RedBlackNodeFactory *const restrict factory)
{
}
#endif


static inline void
rbi_reset(struct RedBlackItorCursor *restrict cursor,
	  struct RedBlackItorNode *restrict path_cursor,
	  struct RedBlackItorNode *restrict *restrict stack_cursor,
	  const struct RedBlackItorController *const restrict controller,
	  struct RedBlackNode *restrict *restrict tree)
{
	struct RedBlackNode *restrict node;
	size_t prev_offset;
	int prev_direction;
	int parent_is_red;
	int parent_info;

	node = *tree;

	/* set cursors to most prev (least/left for asc, greatest/right for
	 * desc) node, keep track of stack */
	if (node != NULL) {
		++stack_cursor;

		*stack_cursor = path_cursor;

		path_cursor->tree        = tree;
		path_cursor->node        = node;
		path_cursor->parent_info = -1; /* root has no parent */

		prev_offset = controller->prev.offset;

		tree = RED_BLACK_LINK_OFFSET(node,
					     prev_offset);
		node = *tree;

		if (node != NULL) {
			prev_direction = controller->prev.direction;
			parent_info    = prev_direction; /* root always black */

			while (1) {
				++path_cursor;
				++stack_cursor;
				*stack_cursor = path_cursor;

				path_cursor->tree	 = tree;
				path_cursor->node	 = node;
				path_cursor->parent_info = parent_info;

				parent_is_red = (int) node->is_red; /* 0, 1 */

				tree = RED_BLACK_LINK_OFFSET(node,
							     prev_offset);
				node = *tree;

				if (node == NULL)
					break;

				parent_info = parent_is_red | prev_direction;
			}
		}
	}

	cursor->stack = stack_cursor;
	cursor->path  = path_cursor;
}

static inline void
rb_itor_init(struct RedBlackItor *const restrict itor,
	     struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNodeFactory *const restrict factory,
	     const struct RedBlackItorController *const restrict controller)

{
	struct RedBlackItorNode *restrict *restrict stack_cursor;
	struct RedBlackItorNode *restrict path_cursor;
	struct RedBlackItorCursor *restrict cursor;

	cursor = &itor->cursor;

	itor->controller = controller;
	itor->factory    = factory;

	path_cursor = &itor->path[0];

	stack_cursor  = &itor->stack[0];
	*stack_cursor = NULL; /* mark top of stack */

	rbi_reset(cursor,
		  path_cursor,
		  stack_cursor,
		  controller,
		  tree);
}


void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			struct RedBlackNode *restrict *const restrict tree,
			struct RedBlackNodeFactory *const restrict factory)
{
	static const struct RedBlackItorController asc_controller = {
		.next =  {
			.offset    = offsetof(struct RedBlackNode, right),
			.direction = RBI_PARENT_INFO_DIRECTION_RIGHT
		},
		.prev =  {
			.offset	   = offsetof(struct RedBlackNode, left),
			.direction = RBI_PARENT_INFO_DIRECTION_LEFT
		},
		.restore = &red_black_asc_itor_restore
	};

	rb_itor_init(itor,
		     tree,
		     factory,
		     &asc_controller);
}


void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNodeFactory *const restrict factory)
{
	static const struct RedBlackItorController desc_controller = {
		.next =  {
			.offset	   = offsetof(struct RedBlackNode, left),
			.direction = RBI_PARENT_INFO_DIRECTION_LEFT
		},
		.prev =  {
			.offset	   = offsetof(struct RedBlackNode, right),
			.direction = RBI_PARENT_INFO_DIRECTION_RIGHT
		},
		.restore = &red_black_desc_itor_restore
	};

	rb_itor_init(itor,
		     tree,
		     factory,
		     &desc_controller);
}


void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNodeFactory *const restrict factory)
{
	struct RedBlackItorCursor *restrict cursor;
	const struct RedBlackItorController *restrict controller;

	cursor     = &itor->cursor;
	controller = itor->controller;

	itor->factory = factory;

	rbi_reset(cursor,
		  &itor->path[0],
		  &itor->stack[0],
		  controller,
		  tree);
}

bool
red_black_itor_current(const struct RedBlackItor *const restrict itor,
		       void **const restrict key_ptr)
{
	const struct RedBlackItorNode *restrict current_itor_node;
	bool has_current;

	current_itor_node = *(itor->cursor.stack);
	has_current       = (current_itor_node != NULL);

	if (has_current)
		*key_ptr = (void *) current_itor_node->node->key;

	return has_current;
}


void
red_black_itor_drop(struct RedBlackItor *const restrict itor)
{
	itor->controller->restore(&itor->cursor,
				  itor->factory);
}


void
red_black_itor_skip(struct RedBlackItor *const restrict itor)
{
	struct RedBlackItorCursor *restrict cursor;
	struct RedBlackItorNode *restrict *restrict stack_cursor;
	struct RedBlackItorNode *restrict path_cursor;
	const struct RedBlackItorController *restrict controller;
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackItorNode *restrict next_itor_node;
	struct RedBlackNode *restrict node;
	size_t prev_offset;
	int prev_direction;
	int parent_is_red;

	cursor	     = &itor->cursor;
	stack_cursor = cursor->stack;
	path_cursor  = cursor->path;

	controller = itor->controller;

	node = path_cursor->node;

	parent_is_red = (int) node->is_red;

	tree = RED_BLACK_LINK_OFFSET(node,
				     controller->next.offset);
	node = *tree;

	if (node == NULL) { /* if can't go forward, walk back up stack */
		--stack_cursor;

		next_itor_node = *stack_cursor;

		if (next_itor_node != NULL) /* unwind path if next node */
			do {
				--path_cursor;
			} while (path_cursor != next_itor_node);

	} else { /* set next itor node */
		++path_cursor;
		*stack_cursor = path_cursor; /* overwrite stack */

		path_cursor->tree	 = tree;
		path_cursor->node	 = node;
		path_cursor->parent_info = parent_is_red
					 | controller->next.direction;

		prev_offset    = controller->prev.offset;
		prev_direction = controller->prev.direction;

		/* find successor */
		while (1) {
			parent_is_red = (int) node->is_red;

			tree = RED_BLACK_LINK_OFFSET(node,
						     prev_offset);

			node = *tree;

			if (node == NULL)
				break;

			++path_cursor;
			++stack_cursor;
			*stack_cursor = path_cursor;

			path_cursor->tree	 = tree;
			path_cursor->node	 = node;
			path_cursor->parent_info = parent_is_red
						 | prev_direction;
		}
	}

	cursor->stack = stack_cursor;
	cursor->path  = path_cursor;
}


bool
red_black_itor_verify(const struct RedBlackItor *const restrict itor,
		      struct RedBlackNode *const restrict *restrict tree,
		      const RedBlackComparator comparator)
{
	struct RedBlackNode *restrict node;
	const void *current_key;
	struct RedBlackItorNode *const restrict *restrict current_stack_cursor;
	struct RedBlackItorNode *restrict current_path_cursor;
	struct RedBlackItorNode *restrict current_itor_node;
	struct RedBlackItorNode *const restrict *restrict base_stack_cursor;
	const struct RedBlackItorNode *restrict path_cursor;
	int compare;
	int expected_parent_info;

	current_stack_cursor = itor->cursor.stack;
	current_path_cursor  = itor->cursor.path;

	/* ensure stack and path point to same node if nodes remain */
	current_itor_node = *current_stack_cursor;

	if (current_itor_node != current_path_cursor)
		return current_itor_node == NULL;

	path_cursor = &itor->path[0];

	base_stack_cursor = &itor->stack[0];

	/* ensure both cursors point to base position if empty tree */
	node = *tree;

	if (node == NULL)
		return (path_cursor       == current_path_cursor)
		    && (base_stack_cursor == current_stack_cursor);

	/* ensure stack begins with NULL marker */
	if (*base_stack_cursor != NULL)
		return false;

	/* fetch current key */
	current_key = current_path_cursor->node->key;

	expected_parent_info = -1; /* root node has no parent */

	/* traverse tree, ensuring path matches upto current_node */
	while (1) {
		if (   (path_cursor->tree        != tree)
		    || (path_cursor->node        != node)
		    || (path_cursor->parent_info != expected_parent_info))
			return false;

		compare = comparator(current_key,
				     node->key);

		if (compare == 0) /* found node, check current alignment */
			return (path_cursor == current_path_cursor);


		if (compare < 0) {
			expected_parent_info = node->is_red
					     ? RBI_PARENT_INFO_RED_LEFT
					     : RBI_PARENT_INFO_BLACK_LEFT;
			tree = &node->left;

		} else {
			expected_parent_info = node->is_red
					     ? RBI_PARENT_INFO_RED_RIGHT
					     : RBI_PARENT_INFO_BLACK_RIGHT;
			tree = &node->right;
		}

		node = *tree;

		if (node == NULL) /* path is not aligned */
			return false;

		++path_cursor;
	}
}
