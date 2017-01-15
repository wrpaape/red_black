#include "red_black_iterator.h" /* RedBlackIterator, RedBlackIteratorUpdater */
#include <stddef.h>             /* NULL */


/* fetch functions
 * ────────────────────────────────────────────────────────────────────────── */
const struct RedBlackNode *restrict *restrict
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

const struct RedBlackNode *restrict *restrict
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


static inline const struct RedBlackNode *restrict *restrict
rbi_init_asc_cursor(const struct RedBlackNode *restrict *restrict cursor,
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

void
red_black_asc_iterator_init(struct RedBlackIterator *const restrict iterator,
			    const struct RedBlackNode *restrict node)
{
	const struct RedBlackNode *restrict *restrict cursor;

	cursor = &iterator->stack[0];

	*cursor = NULL; /* mark top of stack */

	iterator->cursor = rbi_init_asc_cursor(cursor,
					       node);

	iterator->update = &rbi_update_asc;
}

void
red_black_asc_iterator_set(struct RedBlackIterator *const restrict iterator,
			   const struct RedBlackNode *restrict node)
{
	iterator->cursor = rbi_init_asc_cursor(&iterator->stack[0],
					       node);
}


static inline const struct RedBlackNode *restrict *restrict
rbi_init_desc_cursor(const struct RedBlackNode *restrict *restrict cursor,
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

void
red_black_desc_iterator_init(struct RedBlackIterator *const restrict iterator,
			     const struct RedBlackNode *restrict node)
{
	const struct RedBlackNode *restrict *restrict cursor;

	cursor = &iterator->stack[0];

	*cursor = NULL; /* mark top of stack */

	iterator->cursor = rbi_init_desc_cursor(cursor,
						node);

	iterator->update = &rbi_update_desc;
}

void
red_black_desc_iterator_set(struct RedBlackIterator *const restrict iterator,
			    const struct RedBlackNode *restrict node)
{
	iterator->cursor = rbi_init_desc_cursor(&iterator->stack[0],
						node);
}


bool
red_black_iterator_next(struct RedBlackIterator *const restrict iterator,
			void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(iterator->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		iterator->cursor = iterator->update(iterator->cursor,
						    node);

		*key_ptr = (void *) node->key;
	}

	return has_next;
}
