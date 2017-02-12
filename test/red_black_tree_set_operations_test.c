#include "unity.h"             /* UNITY testing framework */
#include "red_black_tree.h"    /* RedBlackTree, struct RedBlackNode */
#include "int_key.h"           /* int_key_comparator */
#include "red_black_test.h"    /* keys */
#include <stdint.h>	       /* intptr_t */



static RedBlackTree all;	 /* all keys */
static RedBlackTree fizz;	 /* keys divisible by 3 */
static RedBlackTree buzz;	 /* keys divisible by 5 */
static RedBlackTree fizz_buzz_u; /* union of fizz and buzz */
static RedBlackTree fizz_buzz_i; /* intersection of fizz and buzz */
static RedBlackTree fizz_buzz_d; /* distinct keys of fizz and buzz */
static RedBlackTree other;	 /* keys not divisible by 3 or 5 */

static int fizz_count;
static int buzz_count;
static int fizz_buzz_u_count;
static int fizz_buzz_i_count;
static int fizz_buzz_d_count;
static int other_count;


static inline void
init_int_tree(RedBlackTree *const restrict tree)
{
	red_black_tree_init(tree,
			    &int_key_comparator);

	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(tree),
				 "INVALID TREE");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_tree_count(tree),
				       "NON-EMPTY TREE");
}

void
setUp(void)
{
	int i;
	int key;
	bool status;

	init_int_tree(&all);
	init_int_tree(&fizz);
	init_int_tree(&buzz);
	init_int_tree(&fizz_buzz_u);
	init_int_tree(&fizz_buzz_i);
	init_int_tree(&fizz_buzz_d);
	init_int_tree(&other);

	shuffle_keys();

	for (i = 0; i < KEYS_COUNT; ++i) {
		key = keys[i];

		status = red_black_tree_add(&all,
					    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");

		if ((key % 15) == 0)
			status = red_black_tree_add(&fizz,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&buzz,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_u,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_i,
						    (void *) (intptr_t) key);
		else if ((key % 3) == 0)
			status = red_black_tree_add(&fizz,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_u,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_d,
						    (void *) (intptr_t) key);
		else if ((key % 5) == 0)
			status = red_black_tree_add(&buzz,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_u,
						    (void *) (intptr_t) key)
			      && red_black_tree_add(&fizz_buzz_d,
						    (void *) (intptr_t) key);
		else
			status = red_black_tree_add(&other,
						    (void *) (intptr_t) key);

		TEST_ASSERT_TRUE_MESSAGE(status,
					 "OUT OF MEMORY");
	}

	fizz_count	  = (int) red_black_tree_count(&fizz);
	buzz_count	  = (int) red_black_tree_count(&buzz);
	fizz_buzz_u_count = (int) red_black_tree_count(&fizz_buzz_u);
	fizz_buzz_i_count = (int) red_black_tree_count(&fizz_buzz_i);
	fizz_buzz_d_count = (int) red_black_tree_count(&fizz_buzz_d);
	other_count	  = (int) red_black_tree_count(&other);
}

void
tearDown(void)
{
	red_black_tree_destroy(&all);
	red_black_tree_destroy(&fizz);
	red_black_tree_destroy(&buzz);
	red_black_tree_destroy(&fizz_buzz_u);
	red_black_tree_destroy(&fizz_buzz_i);
	red_black_tree_destroy(&fizz_buzz_d);
	red_black_tree_destroy(&other);
}


void
test_red_black_tree_insert_all(void)
{
	bool status;
	int count_inserted;
	RedBlackTree tree;

	init_int_tree(&tree);

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
test_red_black_tree_add_all(void)
{
	bool status;
	int count_inserted;
	RedBlackTree tree;

	init_int_tree(&tree);

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
				&fizz_buzz_d);

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


/* void */
/* test_red_black_tree_disjoint_union(void) */
/* { */
/* 	bool status; */
/* 	int count_disjoint_union; */
/* 	RedBlackTree disjoint_union_tree; */

/* 	/1* test disjoint_union of fizz and buzz *1/ */
/* 	count_disjoint_union */
/* 	= red_black_tree_disjoint_union(&disjoint_union_tree, */
/* 					&fizz, */
/* 					&buzz); */

/* 	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_d_count, */
/* 				      count_disjoint_union, */
/* 				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)"); */

/* 	status = red_black_tree_similar(&disjoint_union_tree, */
/* 					&fizz_buzz_d); */

/* 	TEST_ASSERT_TRUE_MESSAGE(status, */
/* 				 "TREE NOT SIMILAR TO 'FIZZ_BUZZ_I' TREE"); */

/* 	red_black_tree_destroy(&disjoint_union_tree); */

/* 	/1* test disjoint_union of fizz buzz keys and other keys *1/ */
/* 	count_disjoint_union */
/* 	= red_black_tree_disjoint_union(&disjoint_union_tree, */
/* 					&fizz_buzz_u, */
/* 					&other); */

/* 	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT, */
/* 				      count_disjoint_union, */
/* 				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)"); */

/* 	status = red_black_tree_similar(&disjoint_union_tree, */
/* 					&all); */

/* 	TEST_ASSERT_TRUE_MESSAGE(status, */
/* 				 "TREE NOT SIMILAR TO 'ALL' TREE"); */

/* 	red_black_tree_destroy(&disjoint_union_tree); */
/* } */
