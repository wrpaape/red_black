#include "red_black_etor.h" /* RedBlackEtor */
#include <stddef.h>         /* NULL */

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static const struct RedBlackEtorOffset asc_etor_offset = {
	.next = offsetof(struct RedBlackNode, right),
	.prev = offsetof(struct RedBlackNode, left)
};

static const struct RedBlackEtorOffset desc_etor_offset = {
	.next = offsetof(struct RedBlackNode, left),
	.prev = offsetof(struct RedBlackNode, right)
};


/* get node->left|right depending on OFF */
#define RBE_GET_OFFSET(NODE,						\
		       OFF)						\
*((const struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFF)))


static inline const struct RedBlackNode *restrict *restrict
rbe_update(const struct RedBlackNode *restrict *restrict cursor,
	   const struct RedBlackNode *restrict node,
	   const struct RedBlackEtorOffset *const restrict offset)
{
	node = RBE_GET_OFFSET(node,
			      offset->next);

	if (node == NULL)
		return cursor - 1;

	/* find next pre/successor, overwrite cursor, keep track of stack */
	const size_t prev = offset->prev;

	while (1) {
		*cursor = node;

		node = RBE_GET_OFFSET(node,
				      prev);

		if (node == NULL)
			return cursor; /* return pointer to pre/successor */

		++cursor;
	}
}


static inline const struct RedBlackNode *restrict *restrict
rbe_reset(const struct RedBlackNode *restrict *restrict cursor,
	  const struct RedBlackNode *restrict node,
	  const size_t prev)
{
	/* set cursor to beginning node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = RBE_GET_OFFSET(node,
				      prev);
	}

	return cursor;
}


static inline void
rb_etor_init(struct RedBlackEtor *const restrict etor,
		   const struct RedBlackNode *restrict root,
		   const struct RedBlackEtorOffset *const restrict offset)

{
	const struct RedBlackNode *restrict *restrict cursor;

	etor->offset = offset;

	cursor = &etor->stack[0];

	*cursor = NULL; /* mark top of stack */

	etor->cursor = rbe_reset(cursor,
				 root,
				 offset->prev);
}


void
red_black_asc_etor_init(struct RedBlackEtor *const restrict etor,
			const struct RedBlackNode *restrict root)
{
	rb_etor_init(etor,
		     root,
		     &asc_etor_offset);
}


void
red_black_desc_etor_init(struct RedBlackEtor *const restrict etor,
			 const struct RedBlackNode *restrict root)
{
	rb_etor_init(etor,
		     root,
		     &desc_etor_offset);
}


void
red_black_etor_reset(struct RedBlackEtor *const restrict etor,
		     const struct RedBlackNode *restrict root)
{
	etor->cursor = rbe_reset(&etor->stack[0],
				 root,
				 etor->offset->prev);
}


bool
red_black_etor_next(struct RedBlackEtor *const restrict etor,
		    void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(etor->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		etor->cursor = rbe_update(etor->cursor,
					  node,
					  etor->offset);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
