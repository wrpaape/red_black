#include "red_black.h"

static struct RedBlackTree tree;

static inline void
do_verify(void)
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

	if (!red_black_tree_verify(&tree)) {
		*ptr++ = 'N';
		*ptr++ = 'O';
		*ptr++ = 'T';
		*ptr++ = ' ';
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
do_print(void)
{
	if (!red_black_tree_print(&tree,
				  &red_black_int_key_sizer,
				  &red_black_int_key_putter))
		EXIT_ON_FAILURE("write error or OUT OF MEMORY");
}

static inline void
do_find(const void *key)
{
	char buffer[256];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr = '\n'; ++ptr;

	if (red_black_tree_find(&tree,
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

	ptr = red_black_int_key_putter(ptr,
				       key);

	*ptr = '\n'; ++ptr;
	*ptr = '\n'; ++ptr;

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}

static inline bool
insert_mode(void)
{
	char input[128];
	void *key;
	ssize_t size_read;
	int status;

	while (1) {
		GET_INPUT(INSERT,
			  input,
			  size_read);

		if (red_black_int_key_init(&key,
					   &input[0])) {
			status = red_black_tree_insert(&tree,
						       key);

			if (status == 0)
				WRITE_LITERAL("tree NOT updated\n");
			else if (status == 1)
				WRITE_LITERAL("tree updated\n");
			else
				EXIT_ON_FAILURE("OUT OF MEMORY");

		} else {
			WRITE_LITERAL(INVALID_INPUT);
		}
	}
}

static inline bool
delete_mode(void)
{
	char input[128];
	void *key;
	ssize_t size_read;

	while (1) {
		GET_INPUT(DELETE,
			  input,
			  size_read);

		if (red_black_int_key_init(&key,
					   &input[0])) {
			if (red_black_tree_delete(&tree,
						  key))
				WRITE_LITERAL("tree updated\n");
			else
				WRITE_LITERAL("tree NOT updated\n");

		} else {
			WRITE_LITERAL(INVALID_INPUT);
		}
	}
}

static inline bool
find_mode(void)
{
	char input[128];
	void *key;
	ssize_t size_read;

	while (1) {
		GET_INPUT(FIND,
			  input,
			  size_read);

		if (red_black_int_key_init(&key,
					   &input[0]))
			do_find(key);
		else
			WRITE_LITERAL(INVALID_INPUT);
	}
}

int
main(void)
{
	char input[128];

	red_black_tree_init(&tree,
			    &red_black_int_key_comparator);

	while (1) {
		GET_CHAR(MAIN_PROMPT,
			 input);

		switch (input[0]) {
		case 'v':
			do_verify();
			break;

		case 'p':
			do_print();
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
