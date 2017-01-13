#ifndef RED_BLACK_HMAP_COUNT_H_
#define RED_BLACK_HMAP_COUNT_H_

/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct RedBlackHMapCount {
	unsigned int buckets_m1;
	unsigned int entries;
	unsigned int max_capacity;
};


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* initial bucket count, MUST be power of 2 */
#define RBHMC_INIT_BUCKET_COUNT	8


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
red_black_hmap_count_init(struct RedBlackHMapCount *const restrict count);

#endif /* ifndef RED_BLACK_HMAP_COUNT_H_ */
