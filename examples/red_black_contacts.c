#include "red_black_tree.h"	/* RedblackTree|Iterator */
#include "str_key.h"		/* string comparator */
#include <stdlib.h>		/* exit */

struct Contact {
	const char *name;
	const char *phone;
	const char *email;
};

struct ContactNode {
	struct Contact contact;
	struct ContactNode *restrict *restrict prev_ptr;
	struct ContactNode *restrict next;
};


struct ContactNode *
alloc_contact_node(void)
{
	static struct ContactNode buffer[128];
	static struct ContactNode *restrict buffer_alloc       = &buffer[0];
	static struct ContactNode *const restrict buffer_until = &buffer[128];

	struct ContactNode

	if (buffer_alloc < buffer_until) {
	}
}

void
free_contact_node


static char string_buffer[8192];
static char *restrict string_buffer_alloc       = &string_buffer[0];
static char *const restrict string_buffer_until = &string_buffer[sizeof(buffer)];

int
compare_contact_names(const void *key1,
		      const void *key2)
{
	struct Contact *restrict contact1;
	struct Contact *restrict contact2;

	contact1 = (struct Contact *restrict) key1;
	contact2 = (struct Contact *restrict) key2;

	return str_key_comparator(contact1->name,
				  contact2->name);
}


int
main(void)
{
}
