#include <curl/curl.h>
#include <stdio.h>

int http_get(const char* url, char* response) 
{
    CURL* curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    return 0;
}