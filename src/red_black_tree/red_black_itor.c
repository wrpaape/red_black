#include "red_black_tree/red_black_itor.h" /* Itor, Node, offsetof, NULL */


/* get NODE->left|right depending on OFFSET
 * ────────────────────────────────────────────────────────────────────────── */
#define RBI_LINK_OFFSET(NODE,						\
			OFFSET)						\
((struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFFSET)))


static inline const struct RedBlackNode *restrict *restrict
rbi_update(const struct RedBlackNode *restrict *restrict cursor,
	   const struct RedBlackItorController *const restrict controller,
	   const struct RedBlackNode *restrict node)
{
	node = *RBI_LINK_OFFSET(node,
				controller->next);

	if (node == NULL)
		return cursor - 1;

	/* find next pre/successor, overwrite cursor, keep track of stack */
	const size_t offset = controller->prev;

	while (1) {
		*cursor = node;

		node = *RBI_LINK_OFFSET(node,
					offset);

		if (node == NULL)
			return cursor; /* return pointer to pre/successor */

		++cursor;
	}
}


static inline const struct RedBlackNode *restrict *restrict
rbi_reset(const struct RedBlackNode *restrict *restrict cursor,
	  const struct RedBlackItorController *const restrict controller,
	  const struct RedBlackNode *restrict node)
{
	size_t offset;

	/* set cursors to most prev (least/left for asc, greatest/right for
	 * desc) node, keep track of stack */
	if (node != NULL) {
		offset = controller->prev;

		do {
			++cursor;
			*cursor = node;

			node = *RBI_LINK_OFFSET(node,
						offset);
		} while (node != NULL);
	}

	return cursor;
}


static inline void
rb_itor_init(struct RedBlackItor *const restrict itor,
	     const struct RedBlackNode *restrict root,
	     const struct RedBlackItorController *const restrict controller)

{
	const struct RedBlackNode *restrict *restrict cursor;

	itor->controller = controller;

	cursor = &itor->stack[0];

	*cursor = NULL; /* mark top of stack */

	itor->cursor = rbi_reset(cursor,
				 controller,
				 root);
}


void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			const struct RedBlackNode *restrict root)
{
	static const struct RedBlackItorController asc_controller = {
		.next = offsetof(struct RedBlackNode, right),
		.prev = offsetof(struct RedBlackNode, left)
	};

	rb_itor_init(itor,
		     root,
		     &asc_controller);
}


void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 const struct RedBlackNode *restrict root)
{
	static const struct RedBlackItorController desc_controller = {
		.next = offsetof(struct RedBlackNode, left),
		.prev = offsetof(struct RedBlackNode, right)
	};

	rb_itor_init(itor,
		     root,
		     &desc_controller);
}


void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     const struct RedBlackNode *restrict root)
{
	itor->cursor = rbi_reset(&itor->stack[0],
				 itor->controller,
				 root);
}


bool
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr)
{
	const struct RedBlackNode *restrict node;
	bool has_next;

	node     = *(itor->cursor);
	has_next = (node != NULL);

	if (has_next) {
		itor->cursor = rbi_update(itor->cursor,
					  itor->controller,
					  node);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
