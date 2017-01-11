#ifndef RED_BLACK_TEST_H_
#define RED_BLACK_TEST_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>          /* (s)random, exit */
#include <stdio.h>	     /* fprintf */


/* print macros
 * ────────────────────────────────────────────────────────────────────────── */
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


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
/* defines length of array of unique integer keys to be inserted/deleted */
#define KEYS_COUNT 100

extern int keys[KEYS_COUNT];
extern const int *const restrict keys_until;

/* check validity after every potential modification ->
 * O(KEYS_COUNT²), set nonzero for small (upto ~10k) KEYS_COUNT */
#define DO_VERIFY 1

#define STR(X)  #X
#define XSTR(X) STR(X)
#define KC_STR  XSTR(KEYS_COUNT)


#define STARTING_TESTS()						\
WRITE_LITERAL("---\nstarting tests for KEYS_COUNT = " KC_STR "\n---\n")

#define ALL_TESTS_PASSED()						\
WRITE_LITERAL("---\nall tests passed!\n---\n")


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
unsigned int
random_upto(const unsigned int upto);

void
init_keys(void);

void
seed_random(void);

void
shuffle_keys(void);

#endif /* ifndef RED_BLACK_TEST_H_ */
