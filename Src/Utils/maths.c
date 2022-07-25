#include "Utils/maths.h"

int64_t max(int64_t first, int64_t second)
{
	return (first > second) ? first : second;
}

int64_t min(int64_t first, int64_t second)
{
	return (first < second) ? first : second;
}

int numlen(int num)
{
	if (num == 0) {
		return 1;
	}

	int len = 0;
	while (num != 0) {
		num /= 10;
		len++;
	}
	return len;
}
