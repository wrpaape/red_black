#include "red_black.h"

static struct RedBlackNode *restrict tree;

static inline void
do_red_black_verify(void)
{
	char buffer[64];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr++ = '\n';
	*ptr++ = 't';
	*ptr++ = 'r';
	*ptr++ = 'e';
	*ptr++ = 'e';
	*ptr++ = ' ';
	*ptr++ = 'i';
	*ptr++ = 's';
	*ptr++ = ' ';

	if (!red_black_verify(&tree)) {
		*ptr+ = 'N';
		*ptr+ = 'O';
		*ptr+ = 'T';
		*ptr+ = ' ';
	}

	*ptr++ = 'p';
	*ptr++ = 'r';
	*ptr++ = 'o';
	*ptr++ = 'p';
	*ptr++ = 'e';
	*ptr++ = 'r';
	*ptr++ = '\n';
	*ptr++ = '\n';

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}

static inline void
do_find_mode(const struct Key *const restrict key)
{
	char buffer[256];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr = '\n'; ++ptr;

	if (red_black_find(tree,
			   key)) {
		*ptr++ = 'f';
		*ptr++ = 'o';
		*ptr++ = 'u';
		*ptr++ = 'n';
		*ptr++ = 'd';

	} else {
		*ptr++ = 'd';
		*ptr++ = 'i';
		*ptr++ = 'd';
		*ptr++ = ' ';
		*ptr++ = 'n';
		*ptr++ = 'o';
		*ptr++ = 't';
		*ptr++ = ' ';
		*ptr++ = 'f';
		*ptr++ = 'i';
		*ptr++ = 'n';
		*ptr++ = 'd';
	}

	*ptr++ = ' ';
	*ptr++ = 'k';
	*ptr++ = 'e';
	*ptr++ = 'y';
	*ptr++ = ':';
	*ptr++ = ' ';

	ptr = put_key(ptr,
		      key);

	*ptr = '\n'; ++ptr;
	*ptr = '\n'; ++ptr;

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}

static inline bool
insert_mode(void)
{
	unsigned char input[128];
	struct Key *restrict key;
	ssize_t size_read;

	while (1) {
		GET_INPUT(INSERT,
			  input,
			  size_read);

		key = pop_key();

		make_key(key,
			 &input[0],
			 (size_t) size_read);

		red_black_insert(&tree,
				 key);
	}
}

static inline bool
delete_mode(void)
{
	return true;
}

static inline bool
find_mode(void)
{
	unsigned char input[128];
	struct Key key;
	ssize_t size_read;

	while (1) {
		GET_INPUT(FIND,
			  input,
			  size_read);

		input[size_read] = '\0';

		key_init(&key,
			 &input[0],
			 (size_t) size_read);

		do_find_mode(&key);
	}
}

int
main(void)
{
	unsigned char input[128];

	red_black_tree_init(&tree,
			    &red_black_int_key_comparator);

	while (1) {
		GET_CHAR(MAIN_PROMPT,
			 input);

		switch (input[0]) {
		case 'v':
			do_red_black_verify();
			break;

		case 'p':
			red_black_print(tree);
			break;

		case 'd':
			if (delete_mode())
				break;

			goto DONE;

		case 'f':
			if (find_mode())
				break;

			goto DONE;

		case 'i':
			if (insert_mode())
				break;

		case 'q':
			goto DONE;

		default:
			WRITE_LITERAL(INVALID_INPUT);
		}
	}

DONE:
	red_black_tree_destroy(&tree);
	return 0;
}
