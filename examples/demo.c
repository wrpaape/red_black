/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree.h" /* RedBlackTree */
#include "int_key.h"        /* int_key accessors */
#include "examples_io.h"    /* WRITE/READ macros */
#include <stdlib.h>	    /* exit, EXIT_FAILURE|SUCCESS */
#include <unistd.h>	    /* read, write, STDOUT|ERR_FILENO */
#include <errno.h>	    /* errno */



/* prompts
 * ────────────────────────────────────────────────────────────────────────── */
#define INVALID_INPUT	"invalid input\n"
#define MAIN_PROMPT	"insert (i), find (f), delete (d), verify (v), print (p), or  quit (q)\n> "
#define INSERT_PROMPT	"input insert integer, back (b), verify (v), print (p), or quit (q)\n> "
#define FIND_PROMPT	"input find integer, back (b), verify (v), print (p), or quit (q)\n> "
#define DELETE_PROMPT	"input delete integer, back (b), verify (v), print (p), or quit (q)\n> "


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(LITERAL)					\
do {									\
	(void) WRITE(STDERR_FILENO,					\
		     "\n" LITERAL "\n",					\
		     sizeof(LITERAL) + 1);				\
	exit(EXIT_FAILURE);						\
} while (0)

#define WRITE_STDOUT(BUFFER,						\
		     SIZE)						\
do {									\
	if (WRITE(STDOUT_FILENO,					\
		  BUFFER,						\
		  (SIZE)) != (SIZE))					\
		EXIT_ON_FAILURE("write failure");			\
} while (0)

#define WRITE_LITERAL(LITERAL)						\
WRITE_STDOUT(LITERAL,							\
	     sizeof(LITERAL) - 1)

#define READ_INPUT(BUFFER)						\
READ(STDIN_FILENO,							\
     BUFFER,								\
     sizeof(BUFFER) - 1)

#define GET_CHAR(PROMPT,						\
		 BUFFER)						\
WRITE_LITERAL(PROMPT);							\
ssize_t _size_read;							\
do {									\
	_size_read = READ_INPUT(BUFFER);				\
	if (_size_read >= 0) {						\
		if ((_size_read > 0) && (_size_read < 3))		\
			break;						\
		WRITE_LITERAL(INVALID_INPUT);				\
	} else {							\
		EXIT_ON_FAILURE("read failure");			\
	}								\
} while (1)


#define GET_INPUT(MODE,							\
		  BUFFER,						\
		  SIZE_READ)						\
WRITE_LITERAL(MODE ## _PROMPT);						\
SIZE_READ = READ_INPUT(BUFFER);						\
switch (SIZE_READ) {							\
case -1:								\
	EXIT_ON_FAILURE("read failure");				\
case 0:									\
	WRITE_LITERAL(INVALID_INPUT);					\
	continue;							\
case 2:									\
	if (input[1] != '\n')						\
		break;							\
	/* fall through */						\
case 1:									\
	switch (input[0]) {						\
	case 'b':							\
		return true;						\
	case 'q':							\
		return false;						\
	case 'v':							\
		do_verify();						\
		continue;						\
	case 'p':							\
		do_print();						\
		continue;						\
	default:							\
		SIZE_READ = 1;						\
	}								\
	break;								\
default:								\
	--SIZE_READ; /* trim '\n' */					\
}									\
BUFFER[SIZE_READ] = '\0'


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackTree tree;


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
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
				  &int_key_sizer,
				  &int_key_putter))
		EXIT_ON_FAILURE("write error or OUT OF MEMORY");
}

static inline void
do_find(const void *key)
{
	char buffer[256];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr++ = '\n';

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

	ptr = int_key_putter(ptr,
			     key);

	*ptr = '\n'; ++ptr;
	*ptr = '\n'; ++ptr;

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}

static inline bool
int_key_init(void *restrict *const restrict key,
	     const char *const restrict string)
{
	*key = (void *) (intptr_t) strtol(string,
					  NULL,
					  10);

	return (*key != 0) || (errno == 0);
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

		if (int_key_init(&key,
				 &input[0])) {
			status = red_black_tree_insert(&tree,
						       key);

			if (status == 0)
				WRITE_LITERAL("\ntree NOT updated\n\n");
			else if (status == 1)
				WRITE_LITERAL("\ntree updated\n\n");
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

		if (int_key_init(&key,
				 &input[0])) {
			if (red_black_tree_delete(&tree,
						  key))
				WRITE_LITERAL("\ntree updated\n\n");
			else
				WRITE_LITERAL("\ntree NOT updated\n\n");

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

		if (int_key_init(&key,
				 &input[0]))
			do_find(key);
		else
			WRITE_LITERAL(INVALID_INPUT);
	}
}


/* entry point
 * ────────────────────────────────────────────────────────────────────────── */
int
main(void)
{
	char input[128];

	red_black_tree_init(&tree,
			    &int_key_comparator);

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
