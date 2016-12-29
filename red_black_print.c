#include "red_black_print.h"

static inline char *
put_indent(char *restrict buffer,
	   const unsigned int level)
{
	char *const restrict indent_until = buffer + (2 * level);

	while (buffer < indent_until) {
		*buffer = '\t';
		++buffer;
		*buffer = '\t';
		++buffer;
	}

	return buffer;
}

static inline char *
put_red_black_null(char *restrict buffer,
		   const unsigned int level)
{
	buffer = put_indent(buffer,
			    level);

	*buffer = '('; ++buffer;
	*buffer = 'B'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'A'; ++buffer;
	*buffer = 'C'; ++buffer;
	*buffer = 'K'; ++buffer;
	*buffer = ')'; ++buffer;
	*buffer = ' '; ++buffer;
	*buffer = 'N'; ++buffer;
	*buffer = 'U'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = '\n';

	return buffer + 1;
}

static inline char *
put_red_black_node(char *restrict buffer,
		   const unsigned int level,
		   const struct RedBlackNode *const restrict node)
{
	buffer = put_indent(buffer,
			    level);

	*buffer = '('; ++buffer;

	if (node->is_red) {
		*buffer = 'R'; ++buffer;
		*buffer = 'E'; ++buffer;
		*buffer = 'D'; ++buffer;

	} else {
		*buffer = 'B'; ++buffer;
		*buffer = 'L'; ++buffer;
		*buffer = 'A'; ++buffer;
		*buffer = 'C'; ++buffer;
		*buffer = 'K'; ++buffer;
	}

	*buffer = ')'; ++buffer;
	*buffer = ' '; ++buffer;

	buffer = put_key(buffer,
			 node->key);

	*buffer = '\n';

	return buffer + 1;
}

char *
do_red_black_print(char *restrict buffer,
		   const unsigned int level,
		   const struct RedBlackNode *const restrict node)
{
	if (node == NULL)
		return put_red_black_null(buffer,
					  level);

	const unsigned int next_level = level + 1;

	buffer = do_red_black_print(buffer,
				    next_level,
				    node->left);

	buffer = put_red_black_node(buffer,
				    level,
				    node);

	return do_red_black_print(buffer,
				  next_level,
				  node->right);
}


void
red_black_print(const struct RedBlackNode *const restrict tree)
{
	char buffer[4096];

	char *const restrict ptr = do_red_black_print(&buffer[0],
						      0,
						      tree);

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}
