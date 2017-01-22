#include "red_black_mt_test.h" /* threads API */
#include "red_black_test.h"    /* keys, print/exit macros */

/* interval macros
 * ────────────────────────────────────────────────────────────────────────── */
#if (   !defined(THREADS_COUNT)						\
     && (THREADS_COUNT != 2)						\
     && (THREADS_COUNT != 4)						\
     && (THREADS_COUNT != 8))
#error expected THREADS_COUNT of 2, 4, or 8
#endif


#define I0 0
#define I1 (KEYS_COUNT / THREADS_COUNT)

#if (THREADS_COUNT == 2)
#define I2 KEYS_COUNT
#else
#define I2 (2 * I1)
#endif /* if (THREADS_COUNT == 2) */

#define I3 (3 * I1)

#if (THREADS_COUNT == 4)
#define I4 KEYS_COUNT
#else
#define I4 (4 * I1)
#endif /* if (THREADS_COUNT == 4) */

#define I5 (5 * I1)
#define I6 (6 * I1)
#define I7 (7 * I1)
#define I8 KEYS_COUNT



/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const struct KeyInterval intervals[THREADS_COUNT] = {
	[0] = { .from = &keys[I0], .until = &keys[I1] },
	[1] = { .from = &keys[I1], .until = &keys[I2] },
#if (THREADS_COUNT > 2)
	[2] = { .from = &keys[I2], .until = &keys[I3] },
	[3] = { .from = &keys[I3], .until = &keys[I4] },
#if (THREADS_COUNT > 4)
	[4] = { .from = &keys[I4], .until = &keys[I5] },
	[5] = { .from = &keys[I5], .until = &keys[I6] },
	[6] = { .from = &keys[I6], .until = &keys[I7] },
	[7] = { .from = &keys[I7], .until = &keys[I8] }
#endif /*if (THREADS_COUNT > 4) */
#endif /*if (THREADS_COUNT > 2) */
};

const struct KeyInterval total_interval = {
	.from  = &keys[0],
	.until = &keys[KEYS_COUNT]
};



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
