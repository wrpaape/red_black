#ifndef RED_BLACK_LOCK_H_
#define RED_BLACK_LOCK_H_

#ifdef WIN32						       /* use SRWLOCK */
/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <Windows.h> /* slim rw lock API, includes Synchapi.h if needed */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef SRWLOCK RedBlackLock;


/* lock macros (must return 0 on success, failure otherwise)
 * ────────────────────────────────────────────────────────────────────────── */
#define RBL_INIT(LOCK_PTR)		InitializeSRWLock(LOCK_PTR)
#define RBL_LOCK_READ(LOCK_PTR)		(AcquireSRWLockShared(LOCK_PTR),    0)
#define RBL_LOCK_WRITE(LOCK_PTR)	(AcquireSRWLockExclusive(LOCK_PTR), 0)
#define RBL_UNLOCK_READ(LOCK_PTR)	(ReleaseSRWLockShared(LOCK_PTR),    0)
#define RBL_UNLOCK_WRITE(LOCK_PTR)	(ReleaseSRWLockExclusive(LOCK_PTR), 0)
#define RBL_DESTROY(LOCK_PTR)		/* no internal memory allocations */



#else						      /* use pthread_rwlock_t */
/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <pthread.h> /* pthread_rwlock_t, PTHREAD_RWLOCK_INITIALIZER */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef pthread_rwlock_t RedBlackLock;


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
extern const RedBlackLock RBL_PROTOTYPE;


/* lock macros (must return 0 on success, failure otherwise)
 * ────────────────────────────────────────────────────────────────────────── */
#define RBL_INITIALIZER			PTHREAD_RWLOCK_INITIALIZER
#define RBL_INIT(LOCK_PTR)		*(LOCK_PTR) = RBL_PROTOTYPE
#define RBL_LOCK_READ(LOCK_PTR)		pthread_rwlock_rdlock(LOCK_PTR)
#define RBL_LOCK_WRITE(LOCK_PTR)	pthread_rwlock_wrlock(LOCK_PTR)
#define RBL_UNLOCK_READ(LOCK_PTR)	pthread_rwlock_unlock(LOCK_PTR)
#define RBL_UNLOCK_WRITE(LOCK_PTR)	pthread_rwlock_unlock(LOCK_PTR)
#define RBL_DESTROY(LOCK_PTR)		/* no internal memory allocations */
#endif /* ifdef WIN32 */

#endif /* ifndef RED_BLACK_LOCK_H_ */
