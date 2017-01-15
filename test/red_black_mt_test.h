#ifndef RED_BLACK_MT_TEST_H_
#define RED_BLACK_MT_TEST_H_

/* threads API
 * ────────────────────────────────────────────────────────────────────────── */
#ifdef WIN32
#include <Windows.h>

typedef HANDLE Thread;
typedef DWORD ThreadReturn;

#define SPAWN_THREAD(THREAD, THREAD_ROUTINE, ARG)			\
((THREAD) = CreateThread(NULL,						\
			 0,						\
			 THREAD_ROUTINE,				\
			 ARG,						\
			 0,						\
			 NULL) != NULL)

#define WAIT_THREAD(THREAD)						\
(WaitForSingleObject(THREAD,						\
		     INFINITE) == WAIT_OBJECT_0)


#else
#include <pthread.h>

typedef pthread_t Thread;
typedef void *ThreadReturn;

#define SPAWN_THREAD(THREAD, THREAD_ROUTINE, ARG)			\
(pthread_create(&(THREAD),						\
		NULL,							\
		THREAD_ROUTINE,						\
		ARG) == 0)

#define WAIT_THREAD(THREAD)						\
(pthread_join(THREAD,							\
	      NULL) == 0)
#endif /* ifdef WIN32 */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef ThreadReturn
(*ThreadRoutine)(void *arg);

struct KeyInterval {
	int *restrict from;
	const int *restrict until;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
#define THREADS_COUNT	4
extern const struct KeyInterval intervals[THREADS_COUNT];
extern const struct KeyInterval total_interval;


/* external API
 * ────────────────────────────────────────────────────────────────────────── */
void
multi_thread_test(const ThreadRoutine test);

#endif /* ifndef RED_BLACK_MT_TEST_H_ */
