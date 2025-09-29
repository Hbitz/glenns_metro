#include <stdbool.h>
#include <stddef.h>


#include "jansson/jansson.h"

typedef struct
{
	char* data;
    size_t size;

	json_t* response;

} HTTPClient;


int HTTPClient_Init(HTTPClient** _ClientPtr);

json_t* HTTPClient_GET(HTTPClient* _Client, const char* _URL);

void HTTPClient_Dispose(HTTPClient** _ClientPtr);

