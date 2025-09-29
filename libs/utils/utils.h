
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

static inline char* strdup(const char* str)
{
	char* copy = (char*)malloc(strlen(str) + 1);
	if(copy == NULL)
		return NULL;

	strcpy(copy, str);
	return copy;
}

static inline int create_folder(const char* _Path)
{
	#if defined _WIN32
		bool success = CreateDirectory(_Path, NULL);
		if(success == false)
		{
			DWORD err = GetLastError();
			if(err == ERROR_ALREADY_EXISTS)
				return 1;
			else
				return -1;

		}
	#else
		int success = mkdir(_Path, 0777);
		if(success != 0)
		{
			if(errno == EEXIST)
				return 1;
			else
				return -1;

		}
	#endif

	return 0;
}
