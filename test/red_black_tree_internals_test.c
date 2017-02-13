#include "unity.h"                            /* UNITY testing framework */
/* #include "red_black_tree.h"		      /1* Tree *1/ */
#include "red_black_tree/red_black_treeify.h" /* red_black_treeify, Node */
#include "red_black_tree/red_black_verify.h"  /* red_black_verify, JumpBuffer */
#include "int_key.h"                          /* int_key_comparator */
#include "red_black_test.h"		      /* KEY_COUNT */
#include <stdint.h>			      /* intptr_t */


/* static RedBlackTree tree; */
/* static bool key_set[KEYS_COUNT]; */
struct RedBlackNode nodes[KEYS_COUNT];

void
setUp(void)
{
	/* int i; */
	/* void *key; */

	/* red_black_tree_init(&tree, */
	/* 		    &int_key_comparator); */

	/* TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree), */
	/* 			 "INVALID TREE"); */

	/* TEST_ASSERT_EQUAL_UINT_MESSAGE(0, */
	/* 			       red_black_tree_count(&tree), */
	/* 			       "NON-EMPTY TREE"); */

	/* shuffle_keys(); */

	/* for (i = 0; i < KEYS_COUNT; ++i) { */
	/* 	key = (void *) (intptr_t) keys[i]; */

	/* 	TEST_ASSERT_TRUE_MESSAGE(red_black_tree_add(&tree, */
	/* 						    key), */
	/* 				 "OUT OF MEMORY"); */
	/* } */

	/* TEST_ASSERT_TRUE_MESSAGE(red_black_tree_verify(&tree), */
	/* 			 "INVALID TREE"); */

	/* TEST_ASSERT_EQUAL_UINT_MESSAGE(KEYS_COUNT, */
	/* 			       red_black_tree_count(&tree), */
	/* 			       "UNEXPECTED COUNT");{ */
}

void
tearDown(void)
{
	/* red_black_tree_destroy(&tree); */
}


void
test_red_black_treeify(void)
{
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict head;
	RedBlackJumpBuffer jump_buffer;
	bool status;

	node = &nodes[0];
	head = NULL;

	for (int i = KEYS_COUNT - 1; i >= 0; --i) {
		node->key  = (void *) (intptr_t) i;
		node->left = head;

		head = node;

		++node;
	}

	node = red_black_treeify(head,
				 KEYS_COUNT - 1,
				 false); /* BLACK */

	status = (RED_BLACK_SET_JUMP(jump_buffer) == 0)
	      && red_black_verify(node,
				  &int_key_comparator,
				  jump_buffer);

	TEST_ASSERT_TRUE_MESSAGE(status,
				 "INVALID TREE");
}


/* void */
/* test_red_black_concat(void) */
/* { */
/* 	int i; */
/* 	int key; */
/* 	bool *restrict key_set_ptr; */
/* 	struct RedBlackNode *restrict node; */

/* 	(void) red_black_concat(tree.root, */
/* 				&node); */

/* 	while (node != NULL) { */
/* 		key = (int) (intptr_t) node->key; */

/* 		key_set_ptr = &key_set[key]; */

/* 		TEST_ASSERT_FALSE_MESSAGE(*key_set_ptr, */
/* 					  "LOOP DETECTED"); */

/* 		*key_set_ptr = true; */

/* 		node = node->left; */
/* 	} */

/* 	for (i = 0; i < KEYS_COUNT; ++i) */
/* 		TEST_ASSERT_TRUE(key_set[i]); */
/* } */
