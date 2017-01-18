#include "red_black_iterator.h" /* RedBlackIterator, RedBlackIteratorSeeker */
#include <stddef.h>             /* NULL */

/* TODO: rename iter -> enumator, store init function,
 * have blueprint for ascending/descending iter */

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


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
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
		 const struct RedBlackIteratorBlueprint *const restrict bp)

{
	const struct RedBlackNode *restrict *restrict cursor;

	iter->blueprint = bp;

	cursor = &iter->stack[0];

	*cursor = NULL; /* mark top of stack */

	iter->cursor = bp->resetter(cursor,
				    root);
}


void
red_black_asc_iterator_init(struct RedBlackIterator *const restrict iter,
			    const struct RedBlackNode *restrict root)
{
	rb_iterator_init(iter,
			 root,
			 &asc_iterator_blueprint);
}


void
red_black_desc_iterator_init(struct RedBlackIterator *const restrict iter,
			     const struct RedBlackNode *restrict root)
{
	rb_iterator_init(iter,
			 root,
			 &desc_iterator_blueprint);
}


void
red_black_iterator_reset(struct RedBlackIterator *const restrict iter,
			 const struct RedBlackNode *restrict root)
{
	iter->cursor = iter->blueprint->resetter(&iter->stack[0],
						 root);
}


bool
red_black_iterator_next(struct RedBlackIterator *const restrict iter,
			void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(iter->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		iter->cursor = iter->blueprint->updater(iter->cursor,
							node);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
