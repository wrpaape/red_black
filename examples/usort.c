#include "red_black_tree.h" /* RedBlackTree|Itor */
#include "str_key.h"        /* str_key_comparator */
#include <stdio.h>          /* puts, fprintf, EOF */
#include <stdlib.h>         /* exit, EXIT_FAILURE */

/* failure macro */
#define EXIT_ON_FAILURE(args...)					\
(void) fprintf(stderr, args), exit(EXIT_FAILURE)


int
main(int argc,
     char *argv[])
{
	RedBlackTree tree;
	RedBlackTreeItor itor;
	const char *restrict key;

	/* initialize string tree */
	red_black_tree_init(&tree,
			    &str_key_comparator);

	/* insert arguments */
	for (int i = 0; i < argc; ++i)
		if (red_black_tree_insert(&tree,
					  argv[i]) < 0)
			EXIT_ON_FAILURE("insertion failure -- OUT OF MEMORY");

	/* initialize ascending itor */
	red_black_tree_asc_itor_init(&itor,
				     &tree);

	/* print unique arguments in ascending order */
	while (red_black_tree_itor_next(&itor,
					(void **) &key))
		if (puts(key) == EOF)
			EXIT_ON_FAILURE("puts failure");

	/* free tree allocations */
	red_black_tree_destroy(&tree);

	return 0;
}
