#include "red_black_iterator.h" /* RedBlackIterator, RedBlackFetcher */
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

void
red_black_iterator_init_asc(RedBlackIterator *const restrict iterator,
			    const struct RedBlackNode *restrict node)
{
	const struct RedBlackNode *restrict *restrict cursor;

	iterator->update = &rbi_update_asc;

	cursor = &iterator->stack[0];

	*cursor = NULL;

	/* set cursor to min node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = node->left;
	}

	iterator->cursor = cursor;
}

void
red_black_iterator_init_desc(RedBlackIterator *const restrict iterator,
			     const struct RedBlackNode *restrict node)
{
	const struct RedBlackNode *restrict *restrict cursor;

	iterator->update = &rbi_update_desc;

	cursor = &iterator->stack[0];

	*cursor = NULL; /* mark top of stack */

	/* set cursor to max node, keep track of stack */
	while (node != NULL) {
		++cursor;
		*cursor = node;

		node = node->right;
	}

	iterator->cursor = cursor;
}


bool
red_black_iterator_next(RedBlackIterator *const restrict iterator,
			const void **const restrict key_ptr)
{
	const struct RedBlackNode *const restrict node = *(iterator->cursor);

	const bool has_next = (node != NULL);

	if (has_next) {
		*key_ptr = node->key;

		iterator->cursor = iterator->update(iterator->cursor,
						    node);
	}

	return has_next;
}
