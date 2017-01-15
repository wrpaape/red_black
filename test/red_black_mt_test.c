#include "red_black_mt_test.h" /* threads API */
#include "red_black_test.h"    /* print/exit macros */

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
#if (THREADS_COUNT == 4)
#define Q0 0
#define Q1 (KEYS_COUNT / THREADS_COUNT)
#define Q2 (2 * Q1)
#define Q3 (3 * Q1)
#define Q4 KEYS_COUNT

const struct KeyInterval intervals[THREADS_COUNT] = {
	[0] = { .from = &keys[Q0], .until = &keys[Q1] },
	[1] = { .from = &keys[Q1], .until = &keys[Q2] },
	[2] = { .from = &keys[Q2], .until = &keys[Q3] },
	[3] = { .from = &keys[Q3], .until = &keys[Q4] }
};

const struct KeyInterval total_interval = {
	.from  = &keys[Q0],
	.until = &keys[Q4]
};
#undef Q0
#undef Q1
#undef Q2
#undef Q3
#undef Q4

#else
#error expected THREADS_COUNT == 4
#endif /* if (THREADS_COUNT == 4) */


void
multi_thread_test(const ThreadRoutine test)
{
	int i;
	Thread threads[THREADS_COUNT];

	ENTER("multi_thread_test");

	i = 0;

	do {
		if (!SPAWN_THREAD(threads[i],
				  test,
				  (void *) &intervals[i]))
			EXIT_ON_SYS_FAILURE("SPAWN_THREAD failure");


		++i;
	} while (i < THREADS_COUNT);

	i = 0;

	do {
		if (!WAIT_THREAD(threads[i]))
			EXIT_ON_SYS_FAILURE("WAIT_THREAD failure");

		++i;
	} while (i < THREADS_COUNT);

	RETURN("multi_thread_test");
}
