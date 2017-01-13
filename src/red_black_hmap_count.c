#include "red_black_hmap_count.h" /* HMapCount, RBHMC_INIT_BUCKET_COUNT */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* maximum average collision count per bucket without requiring expansion */
#define RBHMC_MAX_AVG_COLLISIONS 8


void
red_black_hmap_count_init(struct RedBlackHMapCount *const restrict count)
{
	count->buckets_m1   = RBHMC_INIT_BUCKET_COUNT - 1;
	count->entries      = 0;
	count->max_capacity = RBHMC_INIT_BUCKET_COUNT
			    * RBHMC_MAX_AVG_COLLISIONS;
}
