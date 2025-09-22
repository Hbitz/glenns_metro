#ifndef JSON_H
#define JSON_H

#include <jansson.h>
#include <stdbool.h>

#include "HTTP.h"
#include "Meteo.h"

typedef struct{
    json_t* root;
    json_t* current_weather;
    json_t* object;
    json_error_t error;
    char *json_str;
}json_data;


int json_parse_data(HTTP* http, Meteo* _Meteo);

int json_save_file(Meteo* _Meteo, const char* city_name);

json_t* is_city_in_file(json_t* array, const char* city_name);

bool json_needs_update(const char* city_name);

int json_get_file(Meteo* _Meteo, const char* city_name);

int json_remove_city(json_t** array, const char* city_name);

#endif // JSON_H