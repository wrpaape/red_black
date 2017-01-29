#include "red_black_itor.h"        /* RedBlackItor */
#include "red_black_link_offset.h" /* asc|desc_link_offset, stddef */


/* TODO: dummy restore functions */
static struct RedBlackItorNode *restrict
rbi_restore_left(struct RedBlackItor *const restrict itor_node)
{
	return itor_node;
}
static struct RedBlackItorNode *restrict
rbi_restore_right(struct RedBlackItor *const restrict itor_node)
{
	return itor_node;
}


static inline void
rbi_update(struct RedBlackItorCursor *restrict cursor,
	   struct RedBlackItorNode *restrict *restrict stack_cursor,
	   struct RedBlackItorNode *restrict path_cursor,
	   const struct RedBlackItorController *const restrict controller,
	   struct RedBlackNode *restrict *restrict tree,
	   struct RedBlackNode *restrict node)
{
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
rb_itor_init(struct RedBlackEtor *const restrict itor,
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
		     struct RedBlackNodeFactory *const restrict factory);
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
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr)
{
	struct RedBlackItorCursor *restrict cursor_ptr;
	struct RedBlackItorNode *restrict *restrict stack_cursor;
	struct RedBlackItorNode *restrict path_cursor;
	const struct RedBlackNode *restrict *restrict tree;
	const struct RedBlackNode *restrict node;
	bool has_next;

	cursor_ptr   = &itor->cursor;
	stack_cursor = cursor_ptr->stack;
	has_next     = (stack_cursor != NULL);

	if (has_next) {
		path_cursor = cursor_ptr->path;

		tree = path_cursor->tree;
		node = path_cursor->node;

		rbi_update(cursor_ptr,
			   stack_cursor,
			   path_cursor,
			   itor->controller,
			   tree,
			   node);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}

void
red_black_itor_drop(struct RedBlackItor *const restrict itor);


bool
