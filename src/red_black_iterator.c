#include "red_black_iterator.h" /* RedBlackIterator, RedBlackIteratorSeeker */
#include <stddef.h>             /* NULL */

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static const struct RedBlackIteratorOffset asc_iterator_offset = {
	.next = offsetof(struct RedBlackNode, right),
	.prev = offsetof(struct RedBlackNode, left)
};

static const struct RedBlackIteratorOffset desc_iterator_offset = {
	.next = offsetof(struct RedBlackNode, left),
	.prev = offsetof(struct RedBlackNode, right)
};


/* get node->left|right depending on OFF */
#define RBI_GET_OFFSET(NODE,						\
		       OFF)						\
*((const struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFF)))


static inline const struct RedBlackNode *restrict *restrict
rbi_update(const struct RedBlackNode *restrict *restrict cursor,
	   const struct RedBlackNode *restrict node,
	   const struct RedBlackIteratorOffset *const restrict offset)
{
	node = RBI_GET_OFFSET(node,
			      offset->next);

	if (node == NULL)
		return cursor - 1;

	/* find next pre/successor, overwrite cursor, keep track of stack */
	const size_t prev = offset->prev;

	while (1) {
		*cursor = node;

		node = RBI_GET_OFFSET(node,
				      prev);

		if (node == NULL)
			return cursor; /* return pointer to pre/successor */

		++cursor;
	}
}


static inline const struct RedBlackNode *restrict *restrict
rbi_reset(const struct RedBlackNode *restrict *restrict cursor,
	  const struct RedBlackNode *restrict node,
	  const size_t prev)
{
	/* set cursor to beginning node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = RBI_GET_OFFSET(node,
				      prev);
	}

	return cursor;
}


static inline void
rb_iterator_init(struct RedBlackIterator *const restrict iter,
		 const struct RedBlackNode *restrict root,
		 const struct RedBlackIteratorOffset *const restrict offset)

{
	const struct RedBlackNode *restrict *restrict cursor;

	iter->offset = offset;

	cursor = &iter->stack[0];

	*cursor = NULL; /* mark top of stack */

	iter->cursor = rbi_reset(cursor,
				 root,
				 offset->prev);
}


void
red_black_asc_iterator_init(struct RedBlackIterator *const restrict iter,
			    const struct RedBlackNode *restrict root)
{
	rb_iterator_init(iter,
			 root,
			 &asc_iterator_offset);
}


void
red_black_desc_iterator_init(struct RedBlackIterator *const restrict iter,
			     const struct RedBlackNode *restrict root)
{
	rb_iterator_init(iter,
			 root,
			 &desc_iterator_offset);
}


void
red_black_iterator_reset(struct RedBlackIterator *const restrict iter,
			 const struct RedBlackNode *restrict root)
{
	iter->cursor = rbi_reset(&iter->stack[0],
				 root,
				 iter->offset->prev);
}


bool
red_black_iterator_next(struct RedBlackIterator *const restrict iter,
			void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(iter->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		iter->cursor = rbi_update(iter->cursor,
					  node,
					  iter->offset);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
