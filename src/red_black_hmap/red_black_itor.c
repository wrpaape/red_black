#include "red_black_tree/red_black_itor.h" /* Itor, HNode, offsetof, NULL */


/* get NODE->left|right depending on OFFSET
 * ────────────────────────────────────────────────────────────────────────── */
#define RBI_LINK_OFFSET(NODE,						\
			OFFSET)						\
((struct RedBlackHNode *restrict *restrict) (((char *) (NODE)) + (OFFSET)))


static inline const struct RedBlackHNode *restrict *restrict
rbi_update(const struct RedBlackHNode *restrict *restrict cursor,
	   const struct RedBlackItorController *const restrict controller,
	   const struct RedBlackHNode *restrict node)
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


static inline const struct RedBlackHNode *restrict *restrict
rbi_reset(const struct RedBlackHNode *restrict *restrict cursor,
	  const struct RedBlackItorController *const restrict controller,
	  const struct RedBlackHNode *restrict node)
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
	     const struct RedBlackHNode *restrict root,
	     const struct RedBlackItorController *const restrict controller)

{
	const struct RedBlackHNode *restrict *restrict cursor;

	itor->controller = controller;

	cursor = &itor->stack[0];

	*cursor = NULL; /* mark top of stack */

	itor->cursor = rbi_reset(cursor,
				 controller,
				 root);
}


void
red_black_asc_itor_init(struct RedBlackItor *const restrict itor,
			const struct RedBlackHNode *restrict root)
{
	static const struct RedBlackItorController asc_controller = {
		.next = offsetof(struct RedBlackHNode, right),
		.prev = offsetof(struct RedBlackHNode, left)
	};

	rb_itor_init(itor,
		     root,
		     &asc_controller);
}


void
red_black_desc_itor_init(struct RedBlackItor *const restrict itor,
			 const struct RedBlackHNode *restrict root)
{
	static const struct RedBlackItorController desc_controller = {
		.next = offsetof(struct RedBlackHNode, left),
		.prev = offsetof(struct RedBlackHNode, right)
	};

	rb_itor_init(itor,
		     root,
		     &desc_controller);
}


void
red_black_itor_reset(struct RedBlackItor *const restrict itor,
		     const struct RedBlackHNode *restrict root)
{
	itor->cursor = rbi_reset(&itor->stack[0],
				 itor->controller,
				 root);
}


bool
red_black_itor_next(struct RedBlackItor *const restrict itor,
		    void **const restrict key_ptr)
{
	const struct RedBlackHNode *restrict node;
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
