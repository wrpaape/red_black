#ifndef RED_BLACK_ITOR_NODE_PARENT_INFO_H_
#define RED_BLACK_ITOR_NODE_PARENT_INFO_H_

/* parent_info macros
 * ────────────────────────────────────────────────────────────────────────── */
/* bit 0          ==     0/1    | bit 1 ==            0/1
 * parent->is_red == false/true | node  == parent->left/right */
#define RBIN_PARENT_INFO_NONE	     -1
#define RBIN_PARENT_INFO_BLACK_LEFT   0
#define RBIN_PARENT_INFO_RED_LEFT     1
#define RBIN_PARENT_INFO_BLACK_RIGHT  2
#define RBIN_PARENT_INFO_RED_RIGHT    3

#define RBIN_PARENT_INFO_DIRECTION_LEFT  0
#define RBIN_PARENT_INFO_DIRECTION_RIGHT 2

#endif /* ifndef RED_BLACK_ITOR_NODE_PARENT_INFO_H_ */
