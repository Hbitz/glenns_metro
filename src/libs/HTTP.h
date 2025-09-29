#ifndef __HTTP_h__
#define __HTTP_h__


#include <stdbool.h>
#include <stddef.h>

typedef struct
{
	char* data;
    size_t size;

} HTTP;


int HTTP_init(HTTP* http);

int HTTP_get(HTTP* http, const char* _URL);

void HTTP_dispose(HTTP* http);

#endif