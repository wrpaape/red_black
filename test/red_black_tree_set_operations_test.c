#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */



static RedBlackTree all;	 /* all keys */
static RedBlackTree fizz;	 /* keys divisible by 3 */
static RedBlackTree fizz_only;	 /* keys divisible by 3 (and not by 5) */
static RedBlackTree buzz;	 /* keys divisible by 5 */
static RedBlackTree buzz_only;	 /* keys divisible by 5 (and not by 3) */
static RedBlackTree fizz_buzz_u; /* union of fizz and buzz */
static RedBlackTree fizz_buzz_i; /* intersection of fizz and buzz */
static RedBlackTree fizz_buzz_s; /* distinct keys of fizz and buzz (sym diff) */
static RedBlackTree other;	 /* keys not divisible by 3 or 5 */

static int fizz_count;
static int fizz_only_count;
static int buzz_count;
static int buzz_only_count;
static int fizz_buzz_u_count;
static int fizz_buzz_i_count;
static int fizz_buzz_s_count;
static int other_count;


static inline void
init_tree(RedBlackTree *const restrict tree)
{
	red_black_tree_init(tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(tree),
				       "NON-EMPTY TREE");
}

static inline void
add_key(RedBlackTree *const restrict tree,
	const int key)
{
	const bool status = red_black_tree_add(tree,
					       (void *) (intptr_t) key);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");
}


void
setUp(void)
{
	int i;
	int key;

	init_tree(&all);
	init_tree(&fizz);	 fizz_count        = 0;
	init_tree(&fizz_only);   fizz_only_count   = 0;
	init_tree(&buzz);	 buzz_count        = 0;
	init_tree(&buzz_only);   buzz_only_count   = 0;
	init_tree(&fizz_buzz_u); fizz_buzz_u_count = 0;
	init_tree(&fizz_buzz_i); fizz_buzz_i_count = 0;
	init_tree(&fizz_buzz_s); fizz_buzz_s_count = 0;
	init_tree(&other);	 other_count       = 0;

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		add_key(&all, key);

		if ((key % 15) == 0) {
			add_key(&fizz,	      key); ++fizz_count;
			add_key(&buzz,	      key); ++buzz_count;
			add_key(&fizz_buzz_u, key); ++fizz_buzz_u_count;
			add_key(&fizz_buzz_i, key); ++fizz_buzz_i_count;

		} else if ((key % 3) == 0) {
			add_key(&fizz,	      key); ++fizz_count;
			add_key(&fizz_only,   key); ++fizz_only_count;
			add_key(&fizz_buzz_u, key); ++fizz_buzz_u_count;
			add_key(&fizz_buzz_s, key); ++fizz_buzz_s_count;

		} else if ((key % 5) == 0) {
			add_key(&buzz,	      key); ++buzz_count;
			add_key(&buzz_only,   key); ++buzz_only_count;
			add_key(&fizz_buzz_u, key); ++fizz_buzz_u_count;
			add_key(&fizz_buzz_s, key); ++fizz_buzz_s_count;

		} else {
			add_key(&other,	      key); ++other_count;
		}
	}
}

void
tearDown(void)
{
	red_black_tree_destroy(&all);
	red_black_tree_destroy(&fizz);
	red_black_tree_destroy(&fizz_only);
	red_black_tree_destroy(&buzz);
	red_black_tree_destroy(&buzz_only);
	red_black_tree_destroy(&fizz_buzz_u);
	red_black_tree_destroy(&fizz_buzz_i);
	red_black_tree_destroy(&fizz_buzz_s);
	red_black_tree_destroy(&other);
}


void
test_red_black_tree_insert_all(void)
{
	bool status;
	int count_inserted;
	RedBlackTree tree;

	init_tree(&tree);

	count_inserted = red_black_tree_insert_all(&tree,
						   &fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&tree,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ' TREE");

	count_inserted = red_black_tree_insert_all(&tree,
						   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_insert_all(&tree,
						   &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_insert_all(&tree,
						   &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_insert_all(&tree,
						   &all);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&tree,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'ALL' TREE");

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_put_all(void)
{
	bool status;
	int count_inserted;
	RedBlackTree tree;

	init_tree(&tree);

	count_inserted = red_black_tree_put_all(&tree,
						&fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&tree,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ' TREE");

	count_inserted = red_black_tree_put_all(&tree,
						&buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_put_all(&tree,
						&fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_put_all(&tree,
						&other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_tree_put_all(&tree,
						&all);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&tree,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'ALL' TREE");

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_add_all(void)
{
	bool status;
	int count_inserted;
	RedBlackTree tree;

	init_tree(&tree);

	status = red_black_tree_add_all(&tree,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_tree_similar(&tree,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ' TREE");

	count_inserted = red_black_tree_insert_all(&tree,
						   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_add_all(&tree,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_tree_similar(&tree,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'ALL' TREE");

	red_black_tree_destroy(&tree);
}


void
test_red_black_tree_delete_all(void)
{
	bool status;
	int count_deleted;

	count_deleted = red_black_tree_delete_all(&all,
						  &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	count_deleted = red_black_tree_delete_all(&all,
						  &fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count - fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	count_deleted = red_black_tree_delete_all(&all,
						  &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	status = red_black_tree_similar(&all,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'OTHER' TREE");

	count_deleted = red_black_tree_delete_all(&all,
						  &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	status = red_black_tree_empty(&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT EMPTY");
}


void
test_red_black_tree_drop_all(void)
{
	bool status;

	/* drop fizz buzz union keys from all */
	red_black_tree_drop_all(&all,
				&fizz_buzz_u);

	status = red_black_tree_similar(&all,
					&other);

	/* other keys should remain */
	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'OTHER' TREE");

	/* drop distinct keys from fizz buzz union */
	red_black_tree_drop_all(&fizz_buzz_u,
				&fizz_buzz_s);

	status = red_black_tree_similar(&fizz_buzz_u,
					&fizz_buzz_i);

	/* intersection keys should remain */
	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_I' TREE");
}


void
test_red_black_tree_union(void)
{
	bool status;
	int count_union;
	RedBlackTree union_tree;

	/* test union of fizz and buzz */
	count_union = red_black_tree_union(&union_tree,
					   &fizz,
					   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_u_count,
				      count_union,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&union_tree,
					&fizz_buzz_u);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_U' TREE");

	red_black_tree_destroy(&union_tree);

	/* test union of fizz buzz keys and other keys */
	count_union = red_black_tree_union(&union_tree,
					   &fizz_buzz_u,
					   &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT,
				      count_union,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&union_tree,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'ALL' TREE");

	red_black_tree_destroy(&union_tree);
}


void
test_red_black_tree_intersection(void)
{
	bool status;
	int count_intersection;
	RedBlackTree intersection_tree;

	/* test intersection of fizz and buzz */
	count_intersection = red_black_tree_intersection(&intersection_tree,
							 &fizz,
							 &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_i_count,
				      count_intersection,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&intersection_tree,
					&fizz_buzz_i);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_I' TREE");

	red_black_tree_destroy(&intersection_tree);

	/* test intersection of fizz buzz keys and other keys */
	count_intersection = red_black_tree_intersection(&intersection_tree,
							 &fizz_buzz_u,
							 &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_intersection,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_empty(&intersection_tree);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT EMPTY");
}


void
test_red_black_tree_difference(void)
{
	bool status;
	int count_difference;
	RedBlackTree difference_tree;

	/* test difference of fizz and fizz buzz intersection */
	count_difference
	= red_black_tree_difference(&difference_tree,
				    &fizz,
				    &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_only_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&difference_tree,
					&fizz_only);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_ONLY' TREE");

	red_black_tree_destroy(&difference_tree);

	/* test difference of buzz and fizz buzz intersection */
	count_difference
	= red_black_tree_difference(&difference_tree,
				    &buzz,
				    &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_only_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&difference_tree,
					&buzz_only);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_I' TREE");

	red_black_tree_destroy(&difference_tree);

	/* test difference of all keys and fizz buzz keys */
	count_difference
	= red_black_tree_difference(&difference_tree,
				    &all,
				    &fizz_buzz_u);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&difference_tree,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'OTHER' TREE");

	red_black_tree_destroy(&difference_tree);
}


void
test_red_black_tree_sym_difference(void)
{
	bool status;
	int count_sym_difference;
	RedBlackTree sym_difference_tree;

	/* test sym_difference of fizz and buzz */
	count_sym_difference
	= red_black_tree_sym_difference(&sym_difference_tree,
					&fizz,
					&buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_s_count,
				      count_sym_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&sym_difference_tree,
					&fizz_buzz_s);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_I' TREE");

	red_black_tree_destroy(&sym_difference_tree);

	/* test sym_difference of fizz buzz keys and other keys */
	count_sym_difference
	= red_black_tree_sym_difference(&sym_difference_tree,
					&fizz_buzz_u,
					&other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT,
				      count_sym_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_tree_similar(&sym_difference_tree,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "TREE NOT SIMILAR TO 'ALL' TREE");

	red_black_tree_destroy(&sym_difference_tree);
}
