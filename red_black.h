#ifndef RED_BLACK_H_
#define RED_BLACK_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_insert.h"
#include "red_black_delete.h"
#include "red_black_find.h"
#include "red_black_verify.h"
#include "red_black_print.h"

/* prompts
 * ────────────────────────────────────────────────────────────────────────── */
#define INVALID_INPUT	"invalid input\n"
#define MAIN_PROMPT	"insert (i), find (f), delete (d), verify (v), print (p), or  quit (q)\n> "
#define INSERT_PROMPT	"input insert string, back (b), verify (v), print (p), or quit (q)\n> "
#define FIND_PROMPT	"input find string, back (b), verify (v), print (p), or quit (q)\n> "
#define DELETE_PROMPT	"input delete string, back (b), verify (v), print (p), or quit (q)\n> "

/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define WRITE_LITERAL(LITERAL)						\
if (write(STDOUT_FILENO,						\
	  LITERAL,							\
	  sizeof(LITERAL) - 1) < 0)					\
	EXIT_ON_FAILURE("write failure")

#define READ_INPUT(BUFFER)						\
read(STDIN_FILENO,							\
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
SIZE_READ = READ_INPUT(input);						\
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
		do_red_black_verify();					\
		continue;						\
	case 'p':							\
		red_black_print(tree);					\
		continue;						\
	default:							\
		SIZE_READ = 1;						\
	}								\
	break;								\
default:								\
	--SIZE_READ; /* trim '\n' */					\
}

#endif /* ifndef RED_BLACK_H_ */
