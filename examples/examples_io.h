#ifndef EXAMPLES_IO_H_
#define EXAMPLES_IO_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>	    /* exit, EXIT_FAILURE */


/* IO interface
 * ────────────────────────────────────────────────────────────────────────── */
#ifdef WIN32
#	include <ioh> /* _write */
#	define WRITE(FILENO,						\
		     BUFFER,						\
		     SIZE)						\
	_write(FILENO,							\
	       BUFFER,							\
	       (unsigned int) (SIZE))
#	define READ(FILENO,						\
		    BUFFER,						\
		    SIZE)						\
	_read(FILENO,							\
	      BUFFER,							\
	      (unsigned int) (SIZE))
#	define STDOUT_FILENO	0
#	define STDIN_FILENO	1
#	define STDERR_FILENO	2

#else
#	include <unistd.h> /* write, STDOUT|IN|ERR_FILENO */
#	define WRITE(FILENO,						\
		     BUFFER,						\
		     SIZE)						\
	write(FILENO,							\
	      BUFFER,							\
	      SIZE)
#	define READ(FILENO,						\
		    BUFFER,						\
		    SIZE)						\
	read(FILENO,							\
	      BUFFER,							\
	      SIZE)
#endif /* ifdef WIN32 */


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
		  SIZE) != (SIZE))					\
		EXIT_ON_FAILURE("write failure");			\
} while (0)

#define WRITE_LITERAL(LITERAL)						\
WRITE_STDOUT(LITERAL,							\
	     sizeof(LITERAL) - 1)

#define READ_INPUT(BUFFER)						\
READ(STDIN_FILENO,							\
     BUFFER,								\
     sizeof(BUFFER) - 1)

#endif /* ifndef EXAMPLES_IO_H_ */
