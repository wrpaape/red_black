#include "red_black_lock.h" /* RedBlackLock, RBL_INITIALIZER */


#ifndef WIN32
/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
const RedBlackLock RBL_PROTOTYPE = RBL_INITIALIZER;
#endif /* ifdef WIN32 */
