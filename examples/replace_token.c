/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap.h"	/* RedBlackHMap API */
#include "examples_io.h"	/* low level file open/read/write/close */
#include <stdio.h>		/* high level file IO */
#include <stdlib.h>		/* exit, malloc, free */
#include <stdbool.h>		/* bool */
#include <limits.h>		/* UCHAR_MAX */


/* function-like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define STR(X)	#X
#define XSTR(X) STR(X)


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* maximum length for token or replacement */
#define MAX_LENGTH	128

/* default path to rules file */
#define DEFAULT_RULES_PATH	"rules.csv"
/* usage */
#define USAGE								\
"usage:\n"								\
"\treplace [RULES_FILE] [< <IN_FILE>] [> <OUT_FILE>]\n"			\
"\n"									\
"where\n"								\
"\n"									\
"\tRULES_FILE := .csv table with rows of \"<TOKEN>,<REPLACEMENT>\\n\n"	\
"\t              where TOKEN and REPLACEMENT may be no longer than \n"	\
"\t              " XSTR(MAX_LENGTH) " characters, defaults to "		\
"\t              \"" DEFAULT_RULES_PATH "\" if not provided\n"	\
"\tIN_FILE    := target file\n"						\
"\tOUT_FILE   := output file\n"


/* typedefs, struct declaractions
 * ────────────────────────────────────────────────────────────────────────── */
struct Rule {
	unsigned char token[MAX_LENGTH];
	unsigned char replacement[MAX_LENGTH];
};



/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static const bool token_char[UCHAR_MAX + 1] = {
	['a' ... 'z'] = true,
	['A' ... 'Z'] = true,
	['0' ... '9'] = true,
	['_']         = true
};

static RedBlackHMap rules_map;

static struct Rule *rules_buffer;


static inline void
build_rules_map(const char *const restrict rules_path)
{
	int rules_file
	struct StatBuffer stat_buffer;
	unsigned char *restrict rules_file_buffer;
	unsigned char *restrict rules_cursor;
	const unsigned char *restrict rules_cursor_until;
	size_t rule_file_size;
	const char *restrict failure;

	rules_file = OPEN(rules_path,
			  O_RDONLY);

	if (rules_file < 0)
		failure = "open failure", goto FAIL0;

	if (FSTAT(rules_file,
		  &stat_buffer) != 0)
		failure = "fstat failure", goto FAIL1;

	rules_file_size = stat_buffer.st_size;

	rules_file_buffer = malloc(rules_file_size);

	if (rules_file_buffer == NULL)
		failure = "malloc failure", goto FAIL1;

	if (READ(rules_file,
		 rules_file_buffer,
		 rules_file_size) != rules_file_size)
		failure = "read failure", goto FAIL2;

	if (CLOSE(rules_file) != 0)
		failure = "close failure", goto FAIL2;


FAIL2:	free(rules_file_buffer);
FAIL1:	(void) close(rules_file);
FAIL0:	perror(failure);
	exit(EXIT_FAILURE);
}



int
main(int argc,
     char *argv[])
{
	if (argc > 2) {
		(void) WRITE(STDERR_FILENO,
			     USAGE,
			     sizeof(USAGE) - 1);
		return EXIT_FAILURE;
	}

	build_rules_map((argc < 2) ? DEFAULT_RULES_PATH : argv[1]);

	return EXIT_SUCCESS;
}
