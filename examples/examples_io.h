#ifndef RED_BLACK_EXAMPLES_IO_H_
#define RED_BLACK_EXAMPLES_IO_H_


/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <sys/stat.h>		/* fstat */
#include <sys/types.h>		/* needed for read */

#ifdef WIN32
#	include <io.h>		/* _open, _read, _write, _close */
#else
#	include <sys/uio.h>	/* needed for read */
#	include <unistd.h>	/* STDOUT/IN/ERR_FILENO, read, write, close */
#	include <fcntl.h>	/* open */
#endif /* ifdef WIN32 */



/* wrapper macros
 * ────────────────────────────────────────────────────────────────────────── */
#ifdef WIN32
/* open a file */
#	define OPEN(PATH,						\
		    OPEN_FLAG)						\
	_open(PATH,							\
	      OPEN_FLAG)
/* read a file */
#	define READ(FILE_DESCRIPTOR,					\
		    BUFFER,						\
		    SIZE)						\
	_read(FILE_DESCRIPTOR,						\
	      BUFFER,							\
	      (unsigned int) (SIZE))
/* write to a file */
#	define WRITE(FILE_DESCRIPTOR,					\
		     BUFFER,						\
		     SIZE)						\
	_write(FILE_DESCRIPTOR,						\
	       BUFFER,							\
	       (unsigned int) (SIZE))
/* close a file */
#	define CLOSE(FILE_DESCRIPTOR)					\
	_close(FILE_DESCRIPTOR)

/* buffer struct for FSTAT */
#	define StatBuffer _stat

/* fetch info on a file from its file descriptor */
#	define FSTAT(FILE_DESCRIPTOR,					\
		     BUFFER)						\
	_fstat(FILE_DESCRIPTOR,						\
	       BUFFER)

/* standard input, output, error file descriptors */
#	define STDIN_FILENO	0
#	define STDOUT_FILENO	1
#	define STDERR_FILENO	2

#else
/* open a file */
#	define OPEN(PATH,						\
			 OPEN_FLAG)					\
	open(PATH,							\
	     OPEN_FLAG)
/* read a file */
#	define READ(FILE_DESCRIPTOR,					\
		    BUFFER,						\
		    SIZE)						\
	read(FILE_DESCRIPTOR,						\
	     BUFFER,							\
	     (size_t) SIZE)
/* write to a file */
#	define WRITE(FILE_DESCRIPTOR,					\
		     BUFFER,						\
		     SIZE)						\
	write(FILE_DESCRIPTOR,						\
	      BUFFER,							\
	      (size_t) SIZE)
/* close a file */
#	define CLOSE(FILE_DESCRIPTOR)					\
	close(FILE_DESCRIPTOR)

/* buffer struct for FSTAT */
#	define StatBuffer stat

/* fetch info on a file from its file descriptor */
#	define FSTAT(FILE_DESCRIPTOR,					\
		     BUFFER)						\
	fstat(FILE_DESCRIPTOR,						\
	      BUFFER)
#endif /* ifdef WIN32 */
