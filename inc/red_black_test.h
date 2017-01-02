#ifndef RED_BLACK_TEST_H_
#define RED_BLACK_TEST_H_

#include "red_black_tree.h"    /* red_black_insert|find|verify|delete */
#include "red_black_int_key.h" /* red_black_int_key_comparator */
#include <stdlib.h>            /* (s)random, exit */
#include <stdlib.h>            /* (s)random, exit */
#include <time.h>	       /* time */
#include <stdio.h>	       /* printf, fputs */
#include <unistd.h>	       /* write */
#include <signal.h>            /* sigaction */

#define EXIT_ON_SYS_FAILURE(LITERAL)					\
do {									\
	(void) write(STDERR_FILENO,					\
		     "\n" LITERAL "\n",					\
		     sizeof(LITERAL) + 1);				\
	exit(EXIT_FAILURE);						\
} while (0)

#define WRITE_STDOUT(BUFFER,						\
		     SIZE)						\
do {									\
	if (write(STDOUT_FILENO,					\
		  BUFFER,						\
		  SIZE) != (SIZE))					\
		EXIT_ON_SYS_FAILURE("write failure");			\
} while (0)

#define WRITE_LITERAL(LITERAL)						\
WRITE_STDOUT(LITERAL,							\
	     sizeof(LITERAL) - 1)

#define PRINT_TREE()							\
red_black_tree_print(&tree,						\
		     &red_black_int_key_sizer,				\
		     &red_black_int_key_putter)

#define EXIT_ON_TEST_FAILURE(PRINT_ARGS...)				\
do {									\
	if (   PRINT_TREE()						\
	    && (fprintf(stderr, PRINT_ARGS) >= 0))			\
	       exit(EXIT_SUCCESS);					\
	else								\
		EXIT_ON_SYS_FAILURE("write or fprintf failure or OOM");	\
} while (0)

#define TEST_FAILURE(MODE,						\
		     FORMAT,						\
		     ...)						\
EXIT_ON_TEST_FAILURE("\nred_black_" MODE " test failure -- "		\
		     FORMAT "\n", ##__VA_ARGS__)

#define SYS_FAILURE(MODE,						\
		    FORMAT,						\
		    ...)						\
EXIT_ON_SYS_FAILURE("\nred_black_" MODE " system failure -- "		\
		    FORMAT "\n", ##__VA_ARGS__)

#define FN_ENTER(NAME)	WRITE_LITERAL("entering " NAME "\n")
#define FN_EXIT(NAME)	WRITE_LITERAL("exiting " NAME "\n")

#define TEST_PASS(TEST) WRITE_LITERAL("---\ntest_" TEST " passed!\n---\n")


#endif /* ifndef RED_BLACK_TEST_H_ */
