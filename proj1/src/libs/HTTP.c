#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "HTTP.h"
#include <curl/curl.h>

int HTTP_init(HTTP* http)
{
	memset(http, 0, sizeof(HTTP));
	
	
	printf("HTTP initialized\n");
	return 0;

}

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    HTTP* http = (HTTP*)userp;

	char* ptr = NULL;
	int totalSize = http->size + realsize + 1;
	if(http->data == NULL)
		ptr = (char*)malloc(totalSize);
	else
		ptr = realloc(http->data, totalSize);

    if(ptr == NULL) {
        // Out of memory
        return 0;
    }

    http->data = ptr;
    memcpy(&(http->data[http->size]), contents, realsize);
    http->size += realsize;
    http->data[http->size] = 0; // null-terminate
    return realsize;
}

int HTTP_get(HTTP* http, const char* _URL)
{
	CURL* curl = curl_easy_init();
	if(curl == NULL)
	{
		printf("Failed to initialize CURL\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, _URL); //HTTPs://api.open-Meteo.com/v1/forecast?latitude=52.52&longitude=13.41&hourly=temperature_2m
	/*curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);*/
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)http);

	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK)
	{
		printf("CURL request failed: %s\n", curl_easy_strerror(result));
		curl_easy_cleanup(curl);
		return -2;
	}

	printf("CURL response data: %s\n", http->data);


	curl_easy_cleanup(curl);
}

void HTTP_dispose(HTTP* h)
{
	h++;
}

