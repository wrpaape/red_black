#include "red_black_etor.h"        /* RedBlackEtor, Node, offsetof(), NULL */
#include "red_black_link_offset.h" /* RED_BLACK_LINK_OFFSET */


static inline const struct RedBlackNode *restrict *restrict
rbe_update(const struct RedBlackNode *restrict *restrict cursor,
	   const struct RedBlackNode *restrict node,
	   const struct RedBlackEtorController *const restrict controller)
{
	node = *RED_BLACK_LINK_OFFSET(node,
				      controller->next);

	if (node == NULL)
		return cursor - 1;

	/* find next pre/successor, overwrite cursor, keep track of stack */
	const size_t offset = controller->prev;

	while (1) {
		*cursor = node;

		node = *RED_BLACK_LINK_OFFSET(node,
					      offset);

		if (node == NULL)
			return cursor; /* return pointer to pre/successor */

		++cursor;
	}
}


static inline const struct RedBlackNode *restrict *restrict
rbe_reset(const struct RedBlackNode *restrict *restrict cursor,
	  const struct RedBlackNode *restrict node,
	  const struct RedBlackEtorController *const restrict controller)
{
	size_t offset;

	/* set cursor to most prev (least/greatest) node, keep track of stack */
	if (node != NULL) {
		offset = controller->prev;

		do {
			++cursor;
			*cursor = node;

			node = *RED_BLACK_LINK_OFFSET(node,
						      offset);
		} while (node != NULL);
	}

	return cursor;
}


static inline void
rb_etor_init(struct RedBlackEtor *const restrict etor,
	     const struct RedBlackNode *restrict root,
	     const struct RedBlackEtorController *const restrict controller)

{
	const struct RedBlackNode *restrict *restrict cursor;

	etor->controller = controller;

	cursor = &etor->stack[0];

	*cursor = NULL; /* mark top of stack */

	etor->cursor = rbe_reset(cursor,
				 root,
				 controller);
}


void
red_black_asc_etor_init(struct RedBlackEtor *const restrict etor,
			const struct RedBlackNode *restrict root)
{
	static const struct RedBlackEtorController asc_controller = {
		.next = offsetof(struct RedBlackNode, right),
		.prev = offsetof(struct RedBlackNode, left)
	};

	rb_etor_init(etor,
		     root,
		     &asc_controller);
}


void
red_black_desc_etor_init(struct RedBlackEtor *const restrict etor,
			 const struct RedBlackNode *restrict root)
{
	static const struct RedBlackEtorController desc_controller = {
		.next = offsetof(struct RedBlackNode, left),
		.prev = offsetof(struct RedBlackNode, right)
	};

	rb_etor_init(etor,
		     root,
		     &desc_controller);
}


void
red_black_etor_reset(struct RedBlackEtor *const restrict etor,
		     const struct RedBlackNode *restrict root)
{
	etor->cursor = rbe_reset(&etor->stack[0],
				 root,
				 etor->controller);
}


bool
red_black_etor_next(struct RedBlackEtor *const restrict etor,
		    void **const restrict key_ptr)
{
	const struct RedBlackNode *restrict node;
	bool has_next;

	node     = *(etor->cursor);
	has_next = (node != NULL);

	if (has_next) {
		etor->cursor = rbe_update(etor->cursor,
					  node,
					  etor->controller);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
