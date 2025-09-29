#include "HTTPClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "md5.h"
#include "utils.h"


int HTTPClient_Init(HTTPClient** _ClientPtr)
{
	if(_ClientPtr == NULL)
		return -1;

	HTTPClient* client = (HTTPClient*)malloc(sizeof(HTTPClient));
	if(client == NULL)
	{
		printf("Failed to allocate memory for HTTPClient\n");
		return -1;
	}

	create_folder("cache");


	memset(client, 0, sizeof(HTTPClient));

	*(_ClientPtr) = client;

	return 0;

}

static size_t HTTPClient_Write_Callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    HTTPClient* _Client = (HTTPClient*)userp;

	char* ptr = NULL;
	int totalSize = _Client->size + realsize + 1;
	if(_Client->data == NULL)
		ptr = (char*)malloc(totalSize);
	else
		ptr = realloc(_Client->data, totalSize);

    if(ptr == NULL) {
        // Out of memory
		// Is _Client->data still valid here?
        return 0;
    }

    _Client->data = ptr;
    memcpy(&(_Client->data[_Client->size]), contents, realsize);
    _Client->size += realsize;

    return realsize;
}

json_t* HTTPClient_GET(HTTPClient* _Client, const char* _URL)
{
	const char* hash = MD5_HashToString(_URL, strlen(_URL));
	printf("url: %s\nmd5: %s\n", _URL, hash);

	char cache_filepath[256];
	snprintf(cache_filepath, sizeof(cache_filepath), "cache/%s.json", hash);

	struct stat filinfo;
	if (stat(cache_filepath, &filinfo) == 0)
	{
		time_t nu = time(NULL);
		double skillnad = difftime(nu, filinfo.st_mtime);
		if(skillnad < 900) // 15 minutes cache
		{
			printf("Loading cached response from %s\n", cache_filepath);
			json_t* json = json_load_file(cache_filepath, 0, NULL);
			if(json == NULL)
			{
				printf("Failed to parse cached JSON response! Removing cache file.\n");
				remove(cache_filepath);
			}
			else
			{
				_Client->response = json;
				return _Client->response;
			}
		}
	}

	CURL* curl = curl_easy_init();
	if(curl == NULL)
	{
		printf("Failed to initialize CURL\n");
		errno = -1;
		return NULL;
	}

	curl_easy_setopt(curl, CURLOPT_URL, _URL); //https://api.open-Meteo.com/v1/forecast?latitude=52.52&longitude=13.41&hourly=temperature_2m
	/*curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);*/
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HTTPClient_Write_Callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)_Client);


	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK)
	{
		printf("CURL request failed: %s\n", curl_easy_strerror(result));
		curl_easy_cleanup(curl);

		errno = -2;
		return NULL;
	}

	curl_easy_cleanup(curl);

    _Client->data[_Client->size] = 0; // null-terminate

	json_t* json = json_loads(_Client->data, 0, NULL);
	if(json == NULL)
	{
		printf("Failed to parse JSON response\n");
		errno = -3;
		return NULL;
	}

	json_dump_file(json, cache_filepath, JSON_INDENT(4));

	_Client->response = json;

	return _Client->response;
}

void HTTPClient_Dispose(HTTPClient** _ClientPtr)
{
	if(_ClientPtr == NULL || *(_ClientPtr) == NULL)
		return;

	HTTPClient* _Client = *(_ClientPtr);

	if(_Client->data != NULL)
	{
		free(_Client->data);
		_Client->data = NULL;
	}

	_Client->size = 0;

	if(_Client->response != NULL)
	{
		json_decref(_Client->response);
		_Client->response = NULL;
	}

	free(_Client);
	*(_ClientPtr) = NULL;
}

