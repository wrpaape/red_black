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



/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
/* defines length of array of unique integer keys (must be >= 1) */
#define KEYS_COUNT 1000000

extern int keys[KEYS_COUNT];
extern const int *const restrict keys_until;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
unsigned int
random_upto(const unsigned int upto);

void
init_keys(void)
__attribute__((constructor));

void
seed_random(void)
__attribute__((constructor));

void
shuffle_keys(void);

#endif /* ifndef RED_BLACK_TEST_H_ */
