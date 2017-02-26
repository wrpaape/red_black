#include <set>

static int keys[KEYS_COUNT];

int
main(void)
{
	int i;
	int *key;

	std::set<int> set;

	key = &keys[0];
	i   = 0;

	do {
		*key = i;

		set.insert(i);

		++key;
		++i;
	} while (i < KEYS_COUNT);

	return 0;
}
