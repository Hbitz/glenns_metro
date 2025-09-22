
#include <string.h>

static inline char* strdup(const char* str)
{
	char* copy = (char*)malloc(strlen(str) + 1);
	if(copy == NULL)
		return NULL;

	strcpy(copy, str);
	return copy;
}
