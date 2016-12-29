#ifndef RED_BLACK_LOCK_H_
#define RED_BLACK_LOCK_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <pthread.h>	/* pthread_rwlock_t */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef pthread_rwlock_t RedBlackLock;


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static const RedBlackLock RED_BLACK_LOCK_PROTOTYPE = PTHREAD_RWLOCK_INITIALIZER;


/* lock macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_LOCK_INIT(LOCK)	*(LOCK) = RED_BLACK_LOCK_PROTOTYPE
#define RED_BLACK_LOCK_READ(LOCK)	pthread_rwlock_rdlock(LOCK)
#define RED_BLACK_LOCK_WRITE(LOCK)	pthread_rwlock_wrlock(LOCK)
#define RED_BLACK_LOCK_UNLOCK(LOCK)	pthread_rwlock_unlock(LOCK)

#endif /* ifndef RED_BLACK_LOCK_H_ */
