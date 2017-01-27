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


static inline struct RedBlackItorNode *restrict
rbi_update(struct RedBlackItorNode *restrict cursor,
	   const struct RedBlackNode *restrict node,
	   const struct RedBlackEtorController *const restrict controller)
{
	struct RedBlackNode *restrict *restrict tree;

	offset = controller->next.offset;

	tree = RED_BLACK_LINK_OFFSET(node,
				     controller->next.offset);

	node = *tree;

	if (node == NULL)
		return cursor - 1;

	cursor->restore = controller->next.restore;

	/* find next pre/successor, overwrite cursor, keep track of stack */
	const size_t offset                   = controller->prev.offset;
	const RedBlackItorRestoreNode restore = controller->prev.restore;

	while (1) {
		cursor->tree = tree;
		cursor->node = node;

		node = *RED_BLACK_LINK_OFFSET(node,
					      offset);

		if (node == NULL)
			return cursor; /* return pointer to pre/successor */

		++cursor;
	}
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
	     const struct RedBlackItorController *const restrict controller)

{
	struct RedBlackItorNode *restrict *restrict stack_cursor;
	struct RedBlackItorNode *restrict path_cursor;

	itor->controller = controller;

	stack_cursor  = &itor->stack[0];
	*stack_cursor = NULL; /* mark top of stack */

	path_cursor       = &itor->path[0];
	path_cursor->tree = NULL; /* mark top of stack */

	rbi_reset(&itor->cursor,
		  stack_cursor,
		  path_cursor,
		  controller,
		  tree);
}

void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			struct RedBlackNode *restrict *const restrict tree)
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
		     &asc_controller);
}

void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 struct RedBlackNode *restrict *const restrict tree)
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
		     &desc_controller);
}

void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     struct RedBlackNode *restrict *const restrict tree)
{
	rbi_reset(&itor->cursor,
		  &itor->stack[0],
		  &itor->path[0],
		  itor->controller,
		  tree);
}

bool
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr)
{
	struct RedBlackItorNode *restrict itor_node;
	const struct RedBlackNode *restrict node;
	bool has_next;

	itor_node = itor->cursor;
	has_next  = (itor_node->tree != NULL);

	if (has_next) {
		node = itor_node->node;

		itor->cursor = rbi_update(itor_node,
					  node,
					  itor->controller);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}

void
red_black_itor_drop(struct RedBlackItor *const restrict itor);
