#include "red_black_itor.h"        /* RedBlackItor */
#include "red_black_link_offset.h" /* asc|desc_link_offset, stddef */


/* TODO: dummy restore functions */
static struct RedBlackItorNode *restrict
rbi_restore_left(struct RedBlackItorNode *const restrict itor_node)
{
	return itor_node;
}
static struct RedBlackItorNode *restrict
rbi_restore_right(struct RedBlackItorNode *const restrict itor_node)
{
	return itor_node;
}


static inline void
rbi_reset(struct RedBlackItorCursor *restrict cursor,
	  struct RedBlackItorNode *restrict *restrict stack_cursor,
	  struct RedBlackItorNode *restrict path_cursor,
	  const struct RedBlackItorController *const restrict controller,
	  struct RedBlackNode *restrict *restrict tree)
{
	struct RedBlackNode *restrict node;
	size_t offset;
	RedBlackItorRestoreNode restore;

	node = *tree;

	/* set cursors to most prev (least/left for asc, greatest/right for
	 * desc) node, keep track of stack */
	if (node != NULL) {
		offset  = controller->prev.offset;
		restore = controller->prev.restore;

		do {
			++stack_cursor;
			++path_cursor;

			*stack_cursor = path_cursor;

			path_cursor->tree    = tree;
			path_cursor->node    = node;
			path_cursor->restore = restore;

			tree = RED_BLACK_LINK_OFFSET(node,
						     offset);
			node = *tree;
		} while (node != NULL);
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
	struct RedBlackItorCursor *restrict cursor_ptr;

	cursor_ptr = &itor->cursor;

	itor->controller = controller;
	itor->factory    = factory;

	stack_cursor  = &itor->stack[0];
	*stack_cursor = NULL; /* mark top of stack */

	path_cursor       = &itor->path[0];
	path_cursor->tree = NULL; /* mark top of stack */

	rbi_reset(cursor_ptr,
		  stack_cursor,
		  path_cursor,
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
			.offset  = offsetof(struct RedBlackNode, right),
			.restore = &rbi_restore_right
		},
		.prev =  {
			.offset  = offsetof(struct RedBlackNode, left),
			.restore = &rbi_restore_left
		}
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
			.offset  = offsetof(struct RedBlackNode, left),
			.restore = &rbi_restore_left
		},
		.prev =  {
			.offset  = offsetof(struct RedBlackNode, right),
			.restore = &rbi_restore_right
		}
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
	struct RedBlackItorCursor *restrict cursor_ptr;
	const struct RedBlackItorController *restrict controller;

	cursor_ptr = &itor->cursor;
	controller = itor->controller;

	itor->factory = factory;

	rbi_reset(cursor_ptr,
		  &itor->stack[0],
		  &itor->path[0],
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
}


void
red_black_itor_skip(struct RedBlackItor *const restrict itor)
{
	struct RedBlackItorCursor *restrict cursor_ptr;
	struct RedBlackItorNode *restrict *restrict stack_cursor;
	struct RedBlackItorNode *restrict path_cursor;
	const struct RedBlackItorController *restrict controller;
	struct RedBlackNode *restrict *restrict tree;
	struct RedBlackItorNode *restrict next_itor_node;
	struct RedBlackNode *restrict node;
	size_t prev_offset;
	RedBlackItorRestoreNode prev_restore;

	cursor_ptr   = &itor->cursor;
	stack_cursor = cursor_ptr->stack;
	path_cursor  = cursor_ptr->path;

	controller = itor->controller;

	tree = RED_BLACK_LINK_OFFSET(path_cursor->node,
				     controller->next.offset);
	node = *tree;

	if (node == NULL) { /* if can't go forward, walk back up stack */
		--stack_cursor;

		next_itor_node = *stack_cursor;

		if (next_itor_node != NULL) /* unwind path if next node */
			do {
				--path_cursor;
			} while (path_cursor != next_itor_node);

	} else { /* find next successor */
		/* update current restore direction */
		path_cursor->restore = controller->next.restore;

		prev_offset  = controller->prev.offset;
		prev_restore = controller->prev.restore;

		while (1) {
			++path_cursor;
			*stack_cursor = path_cursor; /* overwrite stack */

			path_cursor->tree = tree;
			path_cursor->node = node;

			tree = RED_BLACK_LINK_OFFSET(node,
						     prev_offset);

			node = *tree;

			if (node == NULL)
				break;

			path_cursor->restore = prev_restore;

			++stack_cursor;
		}
	}

	cursor_ptr->stack = stack_cursor;
	cursor_ptr->path  = path_cursor;
}


bool
red_black_itor_verify(const struct RedBlackItor *const restrict itor,
		      struct RedBlackNode *const restrict *restrict tree,
		      const RedBlackComparator comparator)
{
	RedBlackItorRestoreNode restore;
	struct RedBlackNode *const restrict *restrict current_tree;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict current_node;
	const void *current_key;
	struct RedBlackItorNode *const restrict *restrict current_stack_cursor;
	struct RedBlackItorNode *restrict current_path_cursor;
	struct RedBlackItorNode *const restrict *restrict base_stack_cursor;
	const struct RedBlackItorNode *restrict path_cursor;
	bool path_cursor_points_to_base;
	int compare;

	current_stack_cursor = itor->cursor.stack;
	current_path_cursor  = itor->cursor.path;

	/* ensure stack and path point to same node */
	if (*current_stack_cursor != current_path_cursor)
		return false;

	base_stack_cursor = &itor->stack[0];

	/* ensure stack and path begin with NULL markers */
	if (*base_stack_cursor != NULL)
		return false;

	path_cursor = &itor->path[0];

	if (path_cursor->tree != NULL)
		return false;

	node = *tree;

	/* ensure both cursors point to base position if empty tree */
	path_cursor_points_to_base = (path_cursor == current_path_cursor);

	if (node == NULL)
		return path_cursor_points_to_base
		    && (base_stack_cursor == current_stack_cursor);
	else if (path_cursor_points_to_base)
		return false; /* non-empty tree should never point to base */

	/* fetch current tree, node, and key */
	current_tree = current_path_cursor->tree;
	current_node = current_path_cursor->node;
	current_key  = current_node->key;

	/* traverse tree, ensuring path matches upto current_node */
	while (1) {
		++path_cursor;

		if (   (path_cursor->tree != tree)
		    || (path_cursor->node != node))
			return false;

		compare = comparator(node->key,
				     current_key);

		if (compare == 0) /* found node, check current alignment */
			return (path_cursor == current_path_cursor)
			    && (tree        == current_tree)
			    && (node        == current_node);

		if (compare < 0) {
			tree    = &node->left;
			restore = &rbi_restore_left;

		} else {
			tree    = &node->right;
			restore = &rbi_restore_right;
		}

		if (path_cursor->restore != restore)
			return false;

		node = *tree;

		if (node == NULL) /* path is not aligned */
			return false;
	}
}
