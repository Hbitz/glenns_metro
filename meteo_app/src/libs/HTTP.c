#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "HTTP.h"
#include <curl/curl.h>

int HTTP_init(HTTP* http)
{
	if (!http) {
		printf("Invalid HTTP pointer\n");
		return -1;
	}

	memset(http, 0, sizeof(HTTP));
	
	printf("HTTP initialized\n");
	return 0;
}

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    HTTP* http = (HTTP*)userp;

	if (!http || !contents) {
		return 0;  // Error condition
	}

	char* ptr = NULL;
	int totalSize = http->size + realsize + 1;
	if(http->data == NULL)
		ptr = (char*)malloc(totalSize);
	else
		ptr = realloc(http->data, totalSize);

    if(ptr == NULL) {
        printf("Out of memory in HTTP callback\n");
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
	if (!http || !_URL) {
		printf("Invalid parameters to HTTP_get\n");
		return -1;
	}

	CURL* curl = curl_easy_init();
	if(curl == NULL)
	{
		printf("Failed to initialize CURL\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, _URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)http);
	
	// Add timeout and user agent for better reliability
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Weather-App/1.0");

	CURLcode result = curl_easy_perform(curl);
	if(result != CURLE_OK)
	{
		printf("CURL request failed: %s\n", curl_easy_strerror(result));
		curl_easy_cleanup(curl);
		return -2;
	}

	// Check HTTP response code
	long response_code;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
	
	if (response_code != 200) {
		printf("HTTP request failed with code: %ld\n", response_code);
		curl_easy_cleanup(curl);
		return -3;
	}

	printf("CURL response received (%zu bytes)\n", http->size);

	curl_easy_cleanup(curl);
	return 0;  // Success - this was missing!
}

void HTTP_dispose(HTTP* http)
{
	if (!http) return;
	
	if (http->data) {
		free(http->data);
		http->data = NULL;
	}
	http->size = 0;
}