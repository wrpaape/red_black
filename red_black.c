#include "red_black.h"

static struct RedBlackNode *restrict tree;

static inline void
do_red_black_verify(void)
{
	char buffer[64];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr = '\n'; ++ptr;
	*ptr = 't'; ++ptr;
	*ptr = 'r'; ++ptr;
	*ptr = 'e'; ++ptr;
	*ptr = 'e'; ++ptr;
	*ptr = ' '; ++ptr;
	*ptr = 'i'; ++ptr;
	*ptr = 's'; ++ptr;
	*ptr = ' '; ++ptr;

	if (!red_black_verify(tree)) {
		*ptr = 'N'; ++ptr;
		*ptr = 'O'; ++ptr;
		*ptr = 'T'; ++ptr;
		*ptr = ' '; ++ptr;
	}

	*ptr = 'p'; ++ptr;
	*ptr = 'r'; ++ptr;
	*ptr = 'o'; ++ptr;
	*ptr = 'p'; ++ptr;
	*ptr = 'e'; ++ptr;
	*ptr = 'r'; ++ptr;
	*ptr = '\n'; ++ptr;
	*ptr = '\n'; ++ptr;

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
		*ptr = 'f'; ++ptr;
		*ptr = 'o'; ++ptr;
		*ptr = 'u'; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'd'; ++ptr;

	} else {
		*ptr = 'd'; ++ptr;
		*ptr = 'i'; ++ptr;
		*ptr = 'd'; ++ptr;
		*ptr = ' '; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'o'; ++ptr;
		*ptr = 't'; ++ptr;
		*ptr = ' '; ++ptr;
		*ptr = 'f'; ++ptr;
		*ptr = 'i'; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'd'; ++ptr;
	}

	*ptr = ' '; ++ptr;
	*ptr = 'k'; ++ptr;
	*ptr = 'e'; ++ptr;
	*ptr = 'y'; ++ptr;
	*ptr = ':'; ++ptr;
	*ptr = ' '; ++ptr;

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

			return 0;

		case 'f':
			if (find_mode())
				break;

			return 0;

		case 'i':
			if (insert_mode())
				break;

		case 'q':
			return 0;

		default:
			WRITE_LITERAL(INVALID_INPUT);
		}
	}
}
