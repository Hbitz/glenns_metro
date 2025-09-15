#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>

// int used for loop counters and small values where negative matter
// size_t (unsigned int) is specifically meant to represent sizes of objects in memory,
// (array lengths, sizeof results, memory allocation sizes etc)
// size_t's underlying type depends on the platform(whether its 32 or 64bit based)
typedef struct {
    char *data;
    size_t size;
} http;

int http_get(const char *url, http *response);
//void http_response(http *response);

#endif