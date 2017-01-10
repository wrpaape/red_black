#ifndef RED_BLACK_LOCK_H_
#define RED_BLACK_LOCK_H_

#ifdef WIN32
#error TODO: windows readers-writer lock API
/* lock macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_LOCK_INITIALIZER
#define RED_BLACK_LOCK_INIT(LOCK_PTR)
#define RED_BLACK_LOCK_READ(LOCK_PTR)
#define RED_BLACK_LOCK_WRITE(LOCK_PTR)
#define RED_BLACK_LOCK_UNLOCK(LOCK_PTR)
#define RED_BLACK_LOCK_DESTROY(LOCK_PTR)

#else        /* use pthread API */
/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <pthread.h>	/* pthread_rwlock_t, PTHREAD_RWLOCK_INITIALIZER */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef pthread_rwlock_t RedBlackLock;


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern const RedBlackLock RED_BLACK_LOCK_PROTOTYPE;


/* lock macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_LOCK_INITIALIZER	 PTHREAD_RWLOCK_INITIALIZER
#define RED_BLACK_LOCK_INIT(LOCK_PTR)	 *(LOCK_PTR) = RED_BLACK_LOCK_PROTOTYPE
#define RED_BLACK_LOCK_READ(LOCK_PTR)	 pthread_rwlock_rdlock(LOCK_PTR)
#define RED_BLACK_LOCK_WRITE(LOCK_PTR)	 pthread_rwlock_wrlock(LOCK_PTR)
#define RED_BLACK_LOCK_UNLOCK(LOCK_PTR)	 pthread_rwlock_unlock(LOCK_PTR)
#define RED_BLACK_LOCK_DESTROY(LOCK_PTR) /* no internal memory allocations */
#endif /* ifdef WIN32 */

#endif /* ifndef RED_BLACK_LOCK_H_ */
