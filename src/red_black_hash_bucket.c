#include "red_black_hash_bucket.h" /* HashBucket, Allocator, JUMP, LOCK API */
#include "red_black_insert.h"	   /* red_black_insert */
#include "red_black_update.h"	   /* red_black_update */
#include "red_black_delete.h"	   /* red_black_delete */
#include "red_black_remove.h"	   /* red_black_remove */
#include "red_black_find.h"	   /* red_black_find */
#include "red_black_fetch.h"	   /* red_black_fetch */
#include "red_black_count.h"	   /* red_black_count */
#include "red_black_print.h"	   /* red_black_print */
#include "red_black_verify.h"	   /* red_black_verify */


void
red_black_hash_bucket_init(struct RedBlackHashBucket *const restrict bucket)
{
	RED_BLACK_LOCK_INIT(&bucket->lock);

	bucket->root = NULL;

	rba_bucket_allocator_init(&bucket->allocator);
}


int
red_black_hash_bucket_insert(struct RedBlackHashBucket *const restrict bucket,
			     const struct RedBlackHashKey *const restrict key)
{
	RedBlackJumpBuffer jump_buffer;
	int status;

	status = RED_BLACK_SET_JUMP(jump_buffer);

	if (status == 0) {
		if (RED_BLACK_LOCK_WRITE(&buffer->lock) != 0)
			return -2;

		status = red_black_insert(&bucket->root,
					  &red_black_hash_key_comparator,
					  &bucket->allocator,
					  &jump_buffer,
					  key) /* 1, 0 */
	} else {
		status = RED_BLACK_JUMP_3_STATUS(status);
	}

	return (RED_BLACK_UNLOCK(&buffer->lock) == 0)
	     ? status
	     : -2;
}
