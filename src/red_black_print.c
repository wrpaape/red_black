#include "red_black_print.h"  /* stddef included -> NULL */
#include "red_black_malloc.h" /* RED_BLACK_MALLOC|FREE */
#include <unistd.h>	      /* write, STDOUT_FILENO */


#define SIZE_RED_BLACK_NULL(INDENT) (INDENT + sizeof("(BLACK) NULL"))

static inline size_t
size_red_black_node(const struct RedBlackNode *const restrict node,
		    const RedBlackKeySizer key_sizer,
		    const unsigned int indent)
{
	return indent
	     + (node->is_red ? sizeof("(RED) ") : sizeof("(BLACK) "))
	     + key_sizer(node->key);
}


static size_t
size_red_black_tree(const struct RedBlackNode *const restrict node,
		    const RedBlackKeySizer key_sizer,
		    const unsigned int indent)
{
	if (node == NULL)
		return SIZE_RED_BLACK_NULL(indent);

	const unsigned int next_indent = indent + 2;

	return size_red_black_tree(node->left,
				   key_sizer,
				   next_indent)
	     + size_red_black_node(node,
				   key_sizer,
				   indent)
	     + size_red_black_tree(node->right,
				   key_sizer,
				   next_indent);
}


static inline char *
put_indent(char *restrict buffer,
	   const unsigned int indent)
{
	char *const restrict indent_until = buffer + indent;

	while (buffer < indent_until) {
		*buffer++ = '\t';
		*buffer++ = '\t';
	}

	return buffer;
}

static inline char *
put_red_black_null(char *restrict buffer,
		   const unsigned int indent)
{
	buffer = put_indent(buffer,
			    indent);

	*buffer++ = '(';
	*buffer++ = 'B';
	*buffer++ = 'L';
	*buffer++ = 'A';
	*buffer++ = 'C';
	*buffer++ = 'K';
	*buffer++ = ')';
	*buffer++ = ' ';
	*buffer++ = 'N';
	*buffer++ = 'U';
	*buffer++ = 'L';
	*buffer++ = 'L';
	*buffer++ = '\n';

	return buffer;
}



static inline char *
put_red_black_node(char *restrict buffer,
		   const struct RedBlackNode *const restrict node,
		   const RedBlackKeyPutter key_putter,
		   const unsigned int indent)
{
	buffer = put_indent(buffer,
			    indent);

	*buffer++ = '(';

	if (node->is_red) {
		*buffer++ = 'R';
		*buffer++ = 'E';
		*buffer++ = 'D';

	} else {
		*buffer++ = 'B';
		*buffer++ = 'L';
		*buffer++ = 'A';
		*buffer++ = 'C';
		*buffer++ = 'K';
	}

	*buffer++ = ')';
	*buffer++ = ' ';

	buffer = key_putter(buffer,
			    node->key);

	*buffer++ = '\n';

	return buffer;
}


char *
put_red_black_tree(char *restrict buffer,
		   const struct RedBlackNode *const restrict node,
		   const RedBlackKeyPutter key_putter,
		   const unsigned int indent)
{
	if (node == NULL)
		return put_red_black_null(buffer,
					  indent);

	const unsigned int next_indent = indent + 2;

	buffer = put_red_black_tree(buffer,
				    node->left,
				    key_putter,
				    next_indent);

	buffer = put_red_black_node(buffer,
				    node,
				    key_putter,
				    indent);

	return put_red_black_tree(buffer,
				  node->right,
				  key_putter,
				  next_indent);
}

bool
red_black_print(const struct RedBlackNode *const restrict root,
		const RedBlackKeySizer key_sizer,
		const RedBlackKeyPutter key_putter)
{
	bool success;
	size_t size;

	/* traverse tree to size the required buffer */
	size = size_red_black_tree(root,
				   key_sizer,
				   0);

	char *const restrict buffer = RED_BLACK_MALLOC(size);

	success = (buffer != NULL);

	if (success) {
		/* put nodes in order */
		char *const restrict ptr = put_red_black_tree(buffer,
							      root,
							      key_putter,
							      0);

		size = ptr - buffer;

		success = (write(STDOUT_FILENO,
				 buffer,
				 size) == size);

		RED_BLACK_FREE(buffer);
	}

	return success;
}
