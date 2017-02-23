/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_hmap.h"	/* RedBlackHMap API, bool */
#include "examples_io.h"	/* low level file open/read/write/fstat/close */
#include <stdio.h>		/* high level file IO */
#include <stdlib.h>		/* exit, malloc, free */
#include <string.h>		/* strlen, strerror, memset */
#include <errno.h>		/* errno */
#include <limits.h>		/* UCHAR_MAX */


/* function-like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define STR(X)	#X
#define XSTR(X) STR(X)
#define IS_DIGIT(CHAR) (((CHAR) <= '9') && ((CHAR) >= '0'))

#define DEBUG(...) (void) printf(__VA_ARGS__), (void) fflush(stdout)

/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* maximum length for token or replacement */
#define MAX_LENGTH 127
#define MAX_LENGTH_STR XSTR(MAX_LENGTH)

/* default path to rules file */
#define DEFAULT_RULES_PATH "token_replace_rules.csv"

/* rule delimiter */
#define RULES_DELIM ';'

/* usage */
#define USAGE								\
"usage:\n"								\
"\n"									\
"token_replace [RULES_FILE] [< <IN_FILE>] [> <OUT_FILE>]\n"		\
"\n"									\
"where\n"								\
"\n"									\
"\tRULES_FILE := .csv table with rows of \"<TOKEN>,<REPLACEMENT>\\n\n"	\
"\t              where TOKEN and REPLACEMENT may be no longer than \n"	\
"\t              " MAX_LENGTH_STR " characters, defaults to "		\
"\t              \"" DEFAULT_RULES_PATH "\" if not provided\n"		\
"\tIN_FILE    := target file\n"						\
"\tOUT_FILE   := output file\n"


/* typedefs, struct declaractions
 * ────────────────────────────────────────────────────────────────────────── */
struct Rule {
	char token[MAX_LENGTH + 1];
	char replacement[MAX_LENGTH + 1];
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
static struct Rule *restrict rules_buffer;
static char *restrict rules_file_buffer;


/* reads rules file into 'rules_file_buffer' */
static inline size_t
read_rules_file(const char *const restrict rules_path)
{
	int rules_file;
	struct StatBuffer stat_buffer;
	size_t rules_file_size;
	const char *restrict failure;

	rules_file = OPEN(rules_path,
			  O_RDONLY);

	if (rules_file < 0) {
		failure = "open";
		goto FAIL0;
	}

	if (FSTAT(rules_file,
		  &stat_buffer) != 0) {
		failure = "fstat";
		goto FAIL1;
	}

	rules_file_size = stat_buffer.st_size;

	rules_file_buffer = malloc(rules_file_size);

	if (rules_file_buffer == NULL) {
		failure = "malloc";
		goto FAIL1;
	}

	if (READ(rules_file,
		 rules_file_buffer,
		 rules_file_size) != rules_file_size) {
		failure = "read";
		goto FAIL2;
	}

	if (CLOSE(rules_file) != 0) {
		failure = "close";
		goto FAIL2;
	}

	return rules_file_size;

FAIL2:	free(rules_file_buffer);
FAIL1:	(void) close(rules_file);
FAIL0:	(void) fprintf(stderr,
		       "%s failure (%s) while attempting to read from rules "
		       "file \"%s\"\n",
		       failure,
		       strerror(errno),
		       rules_path);
	exit(EXIT_FAILURE);
}




/* counts number of valid rules, clears out invalid rules and prints a warning
 * to STDERR */
static inline size_t
scan_rules_file(char *restrict cursor,
		const char *const restrict cursor_until)
{
	unsigned int line_num;
	size_t rule_count;
	char *restrict rule_start;
	char *restrict replacement_start;
	unsigned int next_char;

#define WARN_BAD_RULE(FORMAT)						\
(void) fprintf(stderr,							\
	       FORMAT " on line %u, ignoring\n",			\
	       line_num)
#define CLEAR_UNTIL_CURSOR()						\
(void) memset(rule_start,						\
	      '\0',							\
	      cursor - rule_start)

	rule_count = 0;

	for (line_num = 1; ; ++line_num) {
		/* get start of next rule */
		while (1) {
			if (cursor == cursor_until)
				return rule_count;

			next_char = (unsigned int) *cursor;

			if (IS_DIGIT(next_char))
				WARN_BAD_RULE("token starts with digit");
			else if (!token_char[next_char])
				WARN_BAD_RULE("leading non-token character");
			else
				break; /* found good start of token */

			/* clear out bad rule, including newline separator */
			while (1) {
				*cursor++ = '\0';

				if (next_char == '\n')
					break;

				if (cursor == cursor_until)
					return rule_count; /* done */

				next_char = (unsigned int) *cursor;
			}
		}

		rule_start = cursor;

		/* traverse token */
		do {
			++cursor;
			if (cursor == cursor_until) {
				WARN_BAD_RULE("partial token");
				CLEAR_UNTIL_CURSOR();
				return rule_count; /* done */
			}

			next_char = (unsigned int) *cursor;
		} while (token_char[next_char]);


		if (next_char != RULES_DELIM) {
			WARN_BAD_RULE("bad token");
			goto CLEAR_RULE_BAD_TOKEN;
		}

		if ((cursor - rule_start) > MAX_LENGTH) {
			WARN_BAD_RULE("token length exceeds " MAX_LENGTH_STR);
CLEAR_RULE_BAD_TOKEN:	CLEAR_UNTIL_CURSOR();

			while (1) {
				*cursor++ = '\0';

				if (next_char == '\n')
					break;

				if (cursor == cursor_until)
					return rule_count; /* done */

				next_char = *cursor;
			}

			continue; /* do not increment rule_count */
		}

		++cursor; /* skip delimiter */

		replacement_start = cursor;

		while (1) {
			if (cursor == cursor_until) {
				if ((cursor - replacement_start) > MAX_LENGTH) {
					WARN_BAD_RULE("replacement length "
						      "exceeds "
						      MAX_LENGTH_STR);
					CLEAR_UNTIL_CURSOR();

				} else {
					++rule_count;
				}

				return rule_count; /* done */
			}

			if (*cursor == '\n') /* end of line */
				break;

			++cursor;
		}

		if ((cursor - replacement_start) > MAX_LENGTH) {
			WARN_BAD_RULE("replacement length exceeds "
				      MAX_LENGTH_STR);
			CLEAR_UNTIL_CURSOR();
			*cursor = '\0';

		} else {
			++rule_count;
		}

		++cursor;
	}
#undef WARN_BAD_RULE
#undef CLEAR_UNTIL_CURSOR
}




static inline bool
update_rules_map(const struct Rule *const restrict rule,
		 const size_t length_token,
		 const char *restrict *const restrict failure)
{
	int status;
	const struct Rule *restrict old_rule;
	void *restrict old_ptr;

	status = red_black_hmap_update(&rules_map,
				       (void *) rule,
				       length_token,
				       (void **) &old_ptr);

	if (status == 1)
		return true;

	if (status < 0) {
		*failure = "red_black_hmap_update failure";
		return false;
	}

	/* token collision, warn of overwrite */

	old_rule = (const struct Rule *) old_ptr;

	(void) fprintf(stderr,
		       "overwriting old rule: %s -> %s\n"
		       "with new rule:        %s -> %s\n",
		       &old_rule->token[0],
		       &old_rule->replacement[0],
		       &rule->token[0],
		       &rule->replacement[0]);

	return true;
}


static inline void
build_rules_map(const char *const restrict rules_path)
{
	size_t rules_file_size;
	size_t rule_count;
	const char *restrict rules_file_end;
	const char *restrict failure;
	const char *restrict cursor;
	const char *restrict token_start;
	char *restrict rule_cursor;
	struct Rule *restrict rule;
	size_t length_token;
	int next_char;

	rules_file_size = read_rules_file(rules_path);
	rules_file_end  = rules_file_buffer + rules_file_size;
	rule_count      = scan_rules_file(rules_file_buffer,
					  rules_file_end);

	if (rule_count == 0) {
		failure = "parsed no valid rules";
		goto FAIL0;
	}

	rules_buffer = calloc(rule_count,
			      sizeof(*rules_buffer));

	if (rules_buffer == NULL) {
		failure = "calloc failure";
		goto FAIL0;
	}

	if (!red_black_hmap_init(&rules_map)) {
		failure = "red_black_hmap_init failure";
		goto FAIL1;
	}

	cursor = rules_file_buffer;
	rule   = rules_buffer;

	while (1) {
		/* skip bad rules */
		while (1) {
			next_char = (int) *cursor;

			if (next_char != '\0')
				break;

			++cursor;
			if (cursor == rules_file_end) {
				free(rules_file_buffer); /* done with file */
				return; /* map is built */
			}
		}

		token_start = cursor;
		rule_cursor = &rule->token[0];

		/* copy token (non-zero length) */
		while (1) {
			*rule_cursor = next_char;

			++cursor;
			next_char = (int) *cursor;

			if (next_char == RULES_DELIM)
				break;

			++rule_cursor;
		}

		length_token = cursor - token_start;
		rule_cursor  = &rule->replacement[0];

		/* copy replacement (may be empty) */
		while (1) {
			++cursor;
			if (cursor == rules_file_end) {
				if (!update_rules_map(rule,
						      length_token,
						      &failure))
					goto FAIL2;

				free(rules_file_buffer); /* done with file */
				return; /* map is built */
			}

			next_char = (int) *cursor;

			if (next_char == '\n')
				break;

			*rule_cursor++ = next_char;
		}

		/* insert next rule */
		if (!update_rules_map(rule,
				      length_token,
				      &failure))
			break;

		++cursor;
		if (cursor == rules_file_end) {
			free(rules_file_buffer); /* done with file */
			return; /* map is built */
		}

		++rule;
	}

/* should only leave main loop on failure */
FAIL2:	red_black_hmap_destroy(&rules_map);
FAIL1:	free(rules_buffer);
FAIL0:	free(rules_file_buffer);
	(void) fprintf(stderr,
		       "failed to build rules map from file \"%s\": %s\n",
		       rules_path,
		       failure);
	exit(EXIT_FAILURE);
}


static inline bool
copy_remainder(const char *restrict copy_from,
	       const size_t copy_length,
	       const char *restrict *const restrict failure)
{
	const bool success = (fwrite(copy_from,
				     sizeof(*copy_from),
				     copy_length,
				     stdout) == copy_length);

	if (!success)
		*failure = "fwrite failure";

	return success;
}

static inline bool
copy_through_token(const char *restrict copy_from,
		   const char *restrict token,
		   const char *restrict cursor,
		   const char *restrict *const restrict failure)
{
	bool success;
	size_t copy_length;
	size_t token_length;
	const struct Rule *restrict rule;
	void *fetched;

	token_length = cursor - token;

	if (   (token_length <= MAX_LENGTH)
	    && red_black_hmap_fetch(&rules_map,
				    token,
				    token_length,
				    &fetched)) {
		/* copy until token start */
		copy_length = token - copy_from;

		if (fwrite(copy_from,
			   sizeof(*copy_from),
			   copy_length,
			   stdout) != copy_length) {
			*failure = "fwrite failure";
			return false;
		}

		rule = (const struct Rule *) fetched;

		/* copy replacement */
		copy_from = &rule->replacement[0];

		copy_length = strlen(copy_from);

	} else {
		/* no rule for token, copy as is */
		copy_length = cursor - copy_from;
	}

	success = (fwrite(copy_from,
			  sizeof(*copy_from),
			  copy_length,
			  stdout) == copy_length);

	if (!success)
		*failure = "fwrite failure";

	return success;
}



static inline bool
process_line(const char *restrict cursor,
	     const size_t length_line,
	     const char *restrict *const restrict failure)
{
	const char *restrict token;
	const char *restrict copy_from;
	const char *restrict cursor_until;
	unsigned int next_char;


	copy_from    = cursor;
	cursor_until = cursor + length_line;

	while (1) {
		next_char = (unsigned int) *cursor;

		/* find start of next token (can't start with digit) */
		while (   !token_char[next_char]
		       || IS_DIGIT(next_char)) {
			++cursor;
			if (cursor == cursor_until)
				return copy_remainder(copy_from,
						      cursor - copy_from,
						      failure);

			next_char = (unsigned int) *cursor;
		}

		token = cursor;

		/* traverse token */
		do {
			++cursor;
			if (cursor == cursor_until)
				return copy_through_token(copy_from,
							  token,
							  cursor,
							  failure);

			next_char = (unsigned int) *cursor;
		} while (token_char[next_char]);

		if (!copy_through_token(copy_from,
					token,
					cursor,
					failure))
			return false;

		copy_from = cursor;

		/* skip non-token character */
		++cursor;
		if (cursor == cursor_until)
			return copy_remainder(copy_from,
					      1,
					      failure);
	}
}


static inline int
replace_tokens(void)
{
	size_t line_capacity;
	ssize_t line_length;
	char *line_buffer;
	const char *restrict failure;

	line_buffer   = NULL;
	line_capacity = 0;

	do {
		/* read in next line from STDIN */
		line_length = getline(&line_buffer,
				      &line_capacity,
				      stdin);

		if (line_length <= 0) {
			if (line_length < 0) {
				failure = "getline";
				break;
			}

			free(line_buffer);
			return EXIT_SUCCESS;
		}

	} while (process_line(line_buffer,
			      (size_t) line_length,
			      &failure));


	free(line_buffer);
	fprintf(stderr,
		"%s failure (%s) while attempting replace tokens\n",
		failure,
		strerror(errno));
	return EXIT_FAILURE;
}



static inline void
destroy_rules_map(void)
{
	red_black_hmap_destroy(&rules_map);
	free(rules_buffer);
}


/* program entry
 * ────────────────────────────────────────────────────────────────────────── */
int
main(int argc,
     char *argv[])
{
	const char *restrict rules_path;
	int exit_status;

	if (argc > 2) {
		(void) WRITE(STDERR_FILENO,
			     USAGE,
			     sizeof(USAGE) - 1);
		return EXIT_FAILURE;
	}

	rules_path = (argc < 2)
		   ? DEFAULT_RULES_PATH
		   : argv[1];

	build_rules_map(rules_path);

	exit_status = replace_tokens();

	destroy_rules_map();

	return exit_status;
}
