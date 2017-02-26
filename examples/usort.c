/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_tree.h" /* RedBlackTree|Itor */
#include "str_key.h"        /* str_key_comparator */
#include <stdio.h>          /* puts, fputs, perror, EOF */
#include <stdlib.h>         /* EXIT_SUCCESS|FAILURE */


static inline int
print_usort(const int argc,
	    char *const argv[],
	    RedBlackTree *const restrict tree)
{
	RedBlackTreeItor itor;
	void *key;

	/* insert arguments */
	for (int i = 0; i < argc; ++i)
		if (red_black_tree_insert(tree,
					  argv[i]) < 0) {
			(void) fputs("insertion failure: OUT OF MEMORY\n",
				     stderr);
			return EXIT_FAILURE;
		}

	/* initialize ascending iterator */
	red_black_tree_asc_itor_init(&itor,
				     tree);

	/* print unique arguments in ascending order */
	while (red_black_tree_itor_next(&itor,
					&key))
		if (puts((const char *) key) == EOF) {
			perror("puts failure");
			return EXIT_FAILURE;
		}

	return EXIT_SUCCESS;
}

int
main(int argc,
     char *argv[])
{
	RedBlackTree tree;

	/* initialize string tree */
	red_black_tree_init(&tree,
			    &str_key_comparator);

	/* traverse argv */
	const int exit_status = print_usort(argc,
					    argv,
					    &tree);

	/* free tree allocations */
	red_black_tree_destroy(&tree);

	return exit_status;
}
