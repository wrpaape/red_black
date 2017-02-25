#include "unity.h"             /* UNITY testing framework */
#include "red_black_hmap.h"    /* RedBlackHMap, struct RedBlackNode */
#include "red_black_test.h"    /* keys */



static RedBlackHMap all;	 /* all keys */
static RedBlackHMap fizz;	 /* keys divisible by 3 */
static RedBlackHMap fizz_only;	 /* keys divisible by 3 (and not by 5) */
static RedBlackHMap buzz;	 /* keys divisible by 5 */
static RedBlackHMap buzz_only;	 /* keys divisible by 5 (and not by 3) */
static RedBlackHMap fizz_buzz_u; /* union of fizz and buzz */
static RedBlackHMap fizz_buzz_i; /* intersection of fizz and buzz */
static RedBlackHMap fizz_buzz_s; /* distinct keys of fizz and buzz (sym diff) */
static RedBlackHMap other;	 /* keys not divisible by 3 or 5 */

static int fizz_count;
static int fizz_only_count;
static int buzz_count;
static int buzz_only_count;
static int fizz_buzz_u_count;
static int fizz_buzz_i_count;
static int fizz_buzz_s_count;
static int other_count;


static inline void
init_hmap(RedBlackHMap *const restrict hmap)
{
	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_init(hmap),
				 "OUT OF MEMORY");

	TEST_ASSERT_TRUE_MESSAGE(red_black_hmap_verify(hmap),
				 "INVALID HMAP");

	TEST_ASSERT_EQUAL_UINT_MESSAGE(0,
				       red_black_hmap_count(hmap),
				       "NON-EMPTY HMAP");
}

static inline void
add_key(RedBlackHMap *const restrict hmap,
	const int *restrict key)
{
	const bool status = red_black_hmap_add(hmap,
					       (void *) key,
					       sizeof(*key));

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");
}


void
setUp(void)
{
	int *restrict key;
	int key_value;

	init_hmap(&all);
	init_hmap(&fizz);
	init_hmap(&fizz_only);
	init_hmap(&buzz);
	init_hmap(&buzz_only);
	init_hmap(&fizz_buzz_u);
	init_hmap(&fizz_buzz_i);
	init_hmap(&fizz_buzz_s);
	init_hmap(&other);

	shuffle_keys();

	for (key = &keys[0]; key < keys_until; ++key) {
		add_key(&all, key);

		key_value = *key;

		if ((key_value % 15) == 0) {
			add_key(&fizz,	      key);
			add_key(&buzz,	      key);
			add_key(&fizz_buzz_u, key);
			add_key(&fizz_buzz_i, key);

		} else if ((key_value % 3) == 0) {
			add_key(&fizz,	      key);
			add_key(&fizz_only,   key);
			add_key(&fizz_buzz_u, key);
			add_key(&fizz_buzz_s, key);

		} else if ((key_value % 5) == 0) {
			add_key(&buzz,	      key);
			add_key(&buzz_only,   key);
			add_key(&fizz_buzz_u, key);
			add_key(&fizz_buzz_s, key);

		} else {
			add_key(&other,	      key);
		}
	}

	fizz_count        = red_black_hmap_count(&fizz);
	fizz_only_count   = red_black_hmap_count(&fizz_only);
	buzz_count        = red_black_hmap_count(&buzz);
	buzz_only_count   = red_black_hmap_count(&buzz_only);
	fizz_buzz_u_count = red_black_hmap_count(&fizz_buzz_u);
	fizz_buzz_i_count = red_black_hmap_count(&fizz_buzz_i);
	fizz_buzz_s_count = red_black_hmap_count(&fizz_buzz_s);
	other_count       = red_black_hmap_count(&other);
}


void
tearDown(void)
{
	red_black_hmap_destroy(&all);
	red_black_hmap_destroy(&fizz);
	red_black_hmap_destroy(&fizz_only);
	red_black_hmap_destroy(&buzz);
	red_black_hmap_destroy(&buzz_only);
	red_black_hmap_destroy(&fizz_buzz_u);
	red_black_hmap_destroy(&fizz_buzz_i);
	red_black_hmap_destroy(&fizz_buzz_s);
	red_black_hmap_destroy(&other);
}


void
test_red_black_hmap_insert_all(void)
{
	bool status;
	int count_inserted;
	RedBlackHMap hmap;

	init_hmap(&hmap);

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&hmap,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ' HMAP");

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &all);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&hmap,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'ALL' HMAP");

	red_black_hmap_destroy(&hmap);
}


void
test_red_black_hmap_put_all(void)
{
	bool status;
	int count_inserted;
	RedBlackHMap hmap;

	init_hmap(&hmap);

	count_inserted = red_black_hmap_put_all(&hmap,
						&fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&hmap,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ' HMAP");

	count_inserted = red_black_hmap_put_all(&hmap,
						&buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_put_all(&hmap,
						&fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_put_all(&hmap,
						&other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	count_inserted = red_black_hmap_put_all(&hmap,
						&all);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&hmap,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'ALL' HMAP");

	red_black_hmap_destroy(&hmap);
}


void
test_red_black_hmap_add_all(void)
{
	bool status;
	int count_inserted;
	RedBlackHMap hmap;

	init_hmap(&hmap);

	status = red_black_hmap_add_all(&hmap,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_hmap_similar(&hmap,
					&fizz);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ' HMAP");

	count_inserted = red_black_hmap_insert_all(&hmap,
						   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_inserted,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_add_all(&hmap,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "OUT OF MEMORY");

	status = red_black_hmap_similar(&hmap,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'ALL' HMAP");

	red_black_hmap_destroy(&hmap);
}


void
test_red_black_hmap_delete_all(void)
{
	bool status;
	int count_deleted;

	count_deleted = red_black_hmap_delete_all(&all,
						  &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	count_deleted = red_black_hmap_delete_all(&all,
						  &fizz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_count - fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	count_deleted = red_black_hmap_delete_all(&all,
						  &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_count - fizz_buzz_i_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	status = red_black_hmap_similar(&all,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'OTHER' HMAP");

	count_deleted = red_black_hmap_delete_all(&all,
						  &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_deleted,
				      "UNEXPECTED COUNT");

	status = red_black_hmap_empty(&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT EMPTY");
}


void
test_red_black_hmap_drop_all(void)
{
	bool status;

	/* drop fizz buzz union keys from all */
	red_black_hmap_drop_all(&all,
				&fizz_buzz_u);

	status = red_black_hmap_similar(&all,
					&other);

	/* other keys should remain */
	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'OTHER' HMAP");

	/* drop distinct keys from fizz buzz union */
	red_black_hmap_drop_all(&fizz_buzz_u,
				&fizz_buzz_s);

	status = red_black_hmap_similar(&fizz_buzz_u,
					&fizz_buzz_i);

	/* intersection keys should remain */
	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_BUZZ_I' HMAP");
}


void
test_red_black_hmap_union(void)
{
	bool status;
	int count_union;
	RedBlackHMap union_hmap;

	/* test union of fizz and buzz */
	count_union = red_black_hmap_union(&union_hmap,
					   &fizz,
					   &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_u_count,
				      count_union,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&union_hmap,
					&fizz_buzz_u);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_BUZZ_U' HMAP");

	red_black_hmap_destroy(&union_hmap);

	/* test union of fizz buzz keys and other keys */
	count_union = red_black_hmap_union(&union_hmap,
					   &fizz_buzz_u,
					   &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT,
				      count_union,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&union_hmap,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'ALL' HMAP");

	red_black_hmap_destroy(&union_hmap);
}


void
test_red_black_hmap_intersection(void)
{
	bool status;
	int count_intersection;
	RedBlackHMap intersection_hmap;

	/* test intersection of fizz and buzz */
	count_intersection = red_black_hmap_intersection(&intersection_hmap,
							 &fizz,
							 &buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_i_count,
				      count_intersection,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&intersection_hmap,
					&fizz_buzz_i);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_BUZZ_I' HMAP");

	red_black_hmap_destroy(&intersection_hmap);

	/* test intersection of fizz buzz keys and other keys */
	count_intersection = red_black_hmap_intersection(&intersection_hmap,
							 &fizz_buzz_u,
							 &other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(0,
				      count_intersection,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_empty(&intersection_hmap);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT EMPTY");
}


void
test_red_black_hmap_difference(void)
{
	bool status;
	int count_difference;
	RedBlackHMap difference_hmap;

	/* test difference of fizz and fizz buzz intersection */
	count_difference
	= red_black_hmap_difference(&difference_hmap,
				    &fizz,
				    &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_only_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&difference_hmap,
					&fizz_only);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_ONLY' HMAP");

	red_black_hmap_destroy(&difference_hmap);

	/* test difference of buzz and fizz buzz intersection */
	count_difference
	= red_black_hmap_difference(&difference_hmap,
				    &buzz,
				    &fizz_buzz_i);

	TEST_ASSERT_EQUAL_INT_MESSAGE(buzz_only_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&difference_hmap,
					&buzz_only);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_BUZZ_I' HMAP");

	red_black_hmap_destroy(&difference_hmap);

	/* test difference of all keys and fizz buzz keys */
	count_difference
	= red_black_hmap_difference(&difference_hmap,
				    &all,
				    &fizz_buzz_u);

	TEST_ASSERT_EQUAL_INT_MESSAGE(other_count,
				      count_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&difference_hmap,
					&other);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'OTHER' HMAP");

	red_black_hmap_destroy(&difference_hmap);
}


void
test_red_black_hmap_sym_difference(void)
{
	bool status;
	int count_sym_difference;
	RedBlackHMap sym_difference_hmap;

	/* test sym_difference of fizz and buzz */
	count_sym_difference
	= red_black_hmap_sym_difference(&sym_difference_hmap,
					&fizz,
					&buzz);

	TEST_ASSERT_EQUAL_INT_MESSAGE(fizz_buzz_s_count,
				      count_sym_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&sym_difference_hmap,
					&fizz_buzz_s);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'FIZZ_BUZZ_I' HMAP");

	red_black_hmap_destroy(&sym_difference_hmap);

	/* test sym_difference of fizz buzz keys and other keys */
	count_sym_difference
	= red_black_hmap_sym_difference(&sym_difference_hmap,
					&fizz_buzz_u,
					&other);

	TEST_ASSERT_EQUAL_INT_MESSAGE(KEYS_COUNT,
				      count_sym_difference,
				      "UNEXPECTED COUNT OR OUT OF MEMORY (-1)");

	status = red_black_hmap_similar(&sym_difference_hmap,
					&all);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "HMAP NOT SIMILAR TO 'ALL' HMAP");

	red_black_hmap_destroy(&sym_difference_hmap);
}
