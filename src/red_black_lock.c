#include "red_black_lock.h" /* RedBlackLock, RED_BLACK_LOCK_INITIALIZER */


#ifdef WIN32
#error TODO: windows readers-writer lock API

#else
/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const RedBlackLock RED_BLACK_LOCK_PROTOTYPE = RED_BLACK_LOCK_INITIALIZER;
#endif /* ifdef WIN32 */
