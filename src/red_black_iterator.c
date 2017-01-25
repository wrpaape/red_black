#include "red_black_iterator.h" /* RedBlackIterator, RedBlackIteratorSeeker */
#include <stddef.h>             /* NULL */

/* updaters
 * ────────────────────────────────────────────────────────────────────────── */
static const struct RedBlackNode *restrict *restrict
rbi_update_asc(const struct RedBlackNode *restrict *restrict cursor,
	       const struct RedBlackNode *restrict node)
{
	node = node->right;

	if (node == NULL)
		return cursor - 1; /* if leaf, unwind one level */

	/* overwrite cursor, keep track of stack */
	while (1) {
		*cursor = node;

		node = node->left;

		if (node == NULL)
			return cursor; /* return pointer to min successor */

		++cursor;
	}
}

static const struct RedBlackNode *restrict *restrict
rbi_update_desc(const struct RedBlackNode *restrict *restrict cursor,
		const struct RedBlackNode *restrict node)
{
	node = node->left;

	if (node == NULL)
		return cursor - 1; /* if leaf, unwind one level */

	/* overwrite cursor, keep track of stack */
	while (1) {
		*cursor = node;

		node = node->right;

		if (node == NULL)
			return cursor; /* return pointer to max predecessor */

		++cursor;
	}
}

#define RBI_GET_OFFSET(NODE,						\
		       OFFSET)						\
*((const struct RedBlackNode *restrict *restrict) (((char *) (NODE)) + (OFFSET)))

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

/* initializers
 * ────────────────────────────────────────────────────────────────────────── */
static const struct RedBlackNode *restrict *restrict
rbi_reset_asc(const struct RedBlackNode *restrict *restrict cursor,
	      const struct RedBlackNode *restrict node)
{
	/* set cursor to min node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = node->left;
	}

	return cursor;
}


static const struct RedBlackNode *restrict *restrict
rbi_reset_desc(const struct RedBlackNode *restrict *restrict cursor,
	       const struct RedBlackNode *restrict node)
{
	/* set cursor to max node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = node->right;
	}

	return cursor;
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



/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static const struct RedBlackIteratorOffset asc_iterator_offset = {
	.prev = offsetof(struct RedBlackNode, left),
	.next = offsetof(struct RedBlackNode, right)
};

static const struct RedBlackIteratorOffset desc_iterator_offset = {
	.prev = offsetof(struct RedBlackNode, right),
	.next = offsetof(struct RedBlackNode, left)
};

static const struct RedBlackIteratorBlueprint asc_iterator_blueprint = {
	.updater  = rbi_update_asc,
	.resetter = rbi_reset_asc
};

static const struct RedBlackIteratorBlueprint desc_iterator_blueprint = {
	.updater  = rbi_update_desc,
	.resetter = rbi_reset_desc
};

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

	/* iter->cursor = iter->offset->resetter(&iter->stack[0], */
	/* 					 root); */
}


bool
red_black_iterator_next(struct RedBlackIterator *const restrict iter,
			void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(iter->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		/* iter->cursor = iter->offset->updater(iter->cursor, */
		/* 					node); */

		iter->cursor = rbi_update(iter->cursor,
					  node,
					  iter->offset);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
