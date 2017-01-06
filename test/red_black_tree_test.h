#ifndef RED_BLACK_TREE_TEST_H_
#define RED_BLACK_TREE_TEST_H_

#include "red_black_tree.h"  /* red_black_insert|find|verify|delete */
#include "int_key.h"         /* int_key_comparator */
#include <stdlib.h>          /* (s)random, exit */
#include <time.h>	     /* time */
#include <stdio.h>	     /* fprintf */
#include <signal.h>          /* sigaction */

#ifdef WIN32
#	include <ioh> /* _write */
#	define WRITE(FILENO,						\
		     BUFFER,						\
		     SIZE)						\
	_write(FILENO,							\
	       BUFFER,							\
	       (unsigned int) (SIZE))
#	define STDOUT_FILENO 0
#	define STDERR_FILENO 2

#else
#	include <unistd.h> /* write, STDOUT|ERR_FILENO */
#	define WRITE(FILENO,						\
		     BUFFER,						\
		     SIZE)						\
	write(FILENO,							\
	      BUFFER,							\
	      SIZE)
#endif /* ifdef WIN32 */

#define EXIT_ON_SYS_FAILURE(LITERAL)					\
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
		  SIZE) != (SIZE))					\
		EXIT_ON_SYS_FAILURE("write failure");			\
} while (0)

#define WRITE_LITERAL(LITERAL)						\
WRITE_STDOUT(LITERAL,							\
	     sizeof(LITERAL) - 1)

#define PRINT_TREE()							\
red_black_tree_print(&tree,						\
		     &int_key_sizer,					\
		     &int_key_putter)

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

#define ENTER(NAME)	WRITE_LITERAL("entering " NAME "\n")
#define RETURN(NAME)	WRITE_LITERAL("returning from " NAME "\n")

#define TEST_PASS(TEST) WRITE_LITERAL("---\ntest_" TEST " passed!\n---\n")


#endif /* ifndef RED_BLACK_TREE_TEST_H_ */
