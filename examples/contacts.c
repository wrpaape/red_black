#include "red_black_tree.h" /* RedblackTree|Iterator */
#include "str_key.h"	    /* string comparator */
#include "examples_io.h"    /* READ/WRITE/EXIT macros, stdlib (malloc/free) */
#include <string.h>	    /* memcpy */


/* string literals
 * ────────────────────────────────────────────────────────────────────────── */
#define CONTACTS_HEADER							    \
"         NAME          |         PHONE         |         EMAIL         \n" \
"=======================|=======================|=======================\n"
#define CONTACTS_HEADER_LENGTH (sizeof(CONTACTS_HEADER) - 1)

#define ENTER_name  "enter name\n> "
#define ENTER_phone "enter phone number\n> "
#define ENTER_email "enter email address\n> "

#define MAX_LENGTH	23
#define MAX_LENGTH_STR	"23"
#define MAX_SIZE	(MAX_LENGTH + 1)

#define FIELD_TOO_LONG						\
"FIELD TOO LONG! max length is " MAX_LENGTH_STR " characters.\n"

#define OVERWRITE_HEADER "overwriting contact with same name:\n"
#define OVERWRITE_HEADER_LENGTH sizeof(OVERWRITE_HEADER)



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Contact {
	char name[MAX_SIZE];  /* keep key as 1st field, degrades to ptr */
	char phone[MAX_SIZE];
	char email[MAX_SIZE];
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static RedBlackTree contacts;
static unsigned int count_contacts;
static struct Contact *scratch;


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define MALLOC(BUFFER,							\
	       SIZE)							\
if (((BUFFER) = malloc(SIZE)) == NULL)					\
	EXIT_ON_FAILURE("OUT OF MEMORY")


#define GET_FIELD(CONTACT_PTR,						\
		  FIELD)						\
do {									\
	ssize_t _size_read;						\
	WRITE_LITERAL(ENTER_ ## FIELD);					\
	_size_read = READ_INPUT((CONTACT_PTR)->FIELD);			\
	if (_size_read < MAX_SIZE) {					\
		(CONTACT_PTR)->FIELD[_size_read - 1] = '\0'; /* cap */	\
		break;							\
	}								\
	WRITE_LITERAL(FIELD_TOO_LONG);					\
} while (1)

#define GET_CONTACT(CONTACT_PTR)					\
GET_FIELD(CONTACT_PTR, name);						\
GET_FIELD(CONTACT_PTR, phone);						\
GET_FIELD(CONTACT_PTR, email)


static inline char *
put_field(char *restrict buffer,
	  const char *restrict field)
{
	char *const restrict buffer_upto = buffer + MAX_LENGTH;

	while (*field != '\0') /* copy field */
		*buffer++ = *field++;

	while (buffer < buffer_upto) /* add padding */
		*buffer++ = ' ';

	return buffer;
}

static inline char *
put_contact(char *restrict buffer,
	    const struct Contact *const restrict contact)
{
	buffer = put_field(buffer,
			   &contact->name[0]);

	*buffer++ = '|'; /* put separator */

	buffer = put_field(buffer,
			   &contact->phone[0]);

	*buffer++ = '|'; /* put separator */

	buffer = put_field(buffer,
			   &contact->email[0]);

	*buffer++ = '\n'; /* put newline */

	return buffer;
}

static inline char *
put_contacts(char *restrict buffer)
{
	RedBlackTreeIterator iterator;
	const struct Contact *contact;

	(void) memcpy(buffer,
		      CONTACTS_HEADER,
		      CONTACTS_HEADER_LENGTH);

	buffer += CONTACTS_HEADER_LENGTH;

	red_black_tree_asc_iterator_init(&iterator,
					 &contacts);

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &contact))
		buffer = put_contact(buffer,
				     contact);

	return buffer;
}

static inline void
print_contacts(void)
{
	size_t size;
	char *restrict buffer;
	char *restrict end_ptr;

	size = CONTACTS_HEADER_LENGTH
	     + (count_contacts * sizeof(struct Contact));

	MALLOC(buffer,
	       size);

	if (buffer == NULL)
		EXIT_ON_FAILURE("OUT OF MEMORY");

	end_ptr = put_contacts(buffer);

	size = end_ptr - buffer;

	WRITE_STDOUT(buffer,
		     size);

	free(buffer);
}

static inline void
add_success(void)
{
	WRITE_LITERAL("successfully added contact\n");
	++count_contacts;
}

static inline void
warn_overwrite(void)
{
	char buffer[OVERWRITE_HEADER_LENGTH + sizeof(struct Contact)];
	char *restrict ptr;
	size_t size;

	(void) memcpy(&buffer[0],
		      OVERWRITE_HEADER,
		      OVERWRITE_HEADER_LENGTH);

	ptr = put_contact(&buffer[OVERWRITE_HEADER_LENGTH],
			  scratch);

	size = ptr - buffer;

	WRITE_STDOUT(&buffer[0],
		     size);
}

static inline void
add_contact(void)
{
	int status;

	if (scratch == NULL)
		MALLOC(scratch,
		       sizeof(struct Contact));

	GET_CONTACT(scratch);

	status = red_black_tree_update(&contacts,
				       (void *) scratch,
				       (void **) &scratch);

	if (status < 0)
		EXIT_ON_FAILURE("OUT OF MEMORY");

	if (status)
		add_success();
	else
		warn_overwrite();
}

static inline void
remove_success(void)
{
	WRITE_LITERAL("successfully removed contact\n");
	--count_contacts;
}

static inline void
remove_dne(void)
{
	WRITE_LITERAL("failed to find contact\n");
	scratch = NULL;
}

static inline void
remove_contact(void)
{
	struct Contact remove_name;

	GET_FIELD(&remove_name,
		  name);

	if (scratch != NULL)
		free(scratch);

	if (red_black_tree_remove(&contacts,
				  (void *) &remove_name,
				  (void **) &scratch))
		remove_success();
	else
		remove_dne();
}


static inline void
setup(void)
{
	red_black_tree_init(&contacts,
			    &str_key_comparator);
}

static inline void
teardown(void)
{
	RedBlackTreeIterator iterator;
	struct Contact *restrict contact;

	red_black_tree_asc_iterator_init(&iterator,
					 &contacts);

	while (red_black_tree_iterator_next(&iterator,
					    (void **) &contact))
		free(contact);

	red_black_tree_destroy(&contacts);
}


int
main(void)
{
	setup();

	for (int i = 0; i < 5; ++i)
		add_contact();

	print_contacts();

	for (int i = 0; i < 5; ++i)
		remove_contact();

	teardown();
}
