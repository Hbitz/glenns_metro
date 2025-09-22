#include "Json.h"
#include <string.h>



int json_parse_data(HTTP* http, Meteo* _Meteo)
{
    json_data data;

    data.json_str = http->data;
    data.root = json_loads(data.json_str, 0, &data.error);
    if(!data.root){
        printf("failed parsing from buffer\n");
        fprintf(stderr, "JSON error: %s\n", data.error.text);
        free(data.json_str);
        return false;
    }
    data.current_weather = json_object_get(data.root, "current_weather");
    if(!data.current_weather){
        printf("failed to find 'current_weather'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    data.object = json_object_get(data.current_weather, "temperature");
    if(!data.object){
        printf("failed to find 'temperature'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->temperature = (float)json_number_value(data.object);
    
    data.object = json_object_get(data.current_weather, "windspeed");
    if(!data.object){
        printf("failed to find 'windspeed'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->wind_speed = (float)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "winddirection");
    if(!data.object){
        printf("failed to find 'winddirection'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->wind_direction = (int)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "is_day");
    if(!data.object){
        printf("failed to find 'is_day'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->is_day = (bool)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "weathercode");
    if(!data.object){
        printf("failed to find 'weathercode'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->weather_code = (int)json_number_value(data.object);

    free(data.json_str);
    json_decref(data.root);
    return 0;
}

int json_get_file(Meteo* _Meteo, const char* city_name)
{
    json_t* cities = NULL;
    json_t* temp = NULL;
    cities = json_load_file("cities.json", 0, NULL);
    if(!cities){
        cities = json_array();
    }
    json_t* object = is_city_in_file(cities, city_name);

    if(object != NULL){
        printf("Found file, getting values...\n");
        temp = json_object_get(object, "temperature");
        _Meteo->temperature = json_real_value(temp);
        temp = json_object_get(object, "windspeed");
        _Meteo->wind_speed = json_real_value(temp);
        temp = json_object_get(object, "winddirection");
        _Meteo->wind_direction = json_integer_value(temp);
        temp = json_object_get(object, "is_day");
        _Meteo->is_day = json_boolean_value(temp);
        temp = json_object_get(object, "weathercode");
        _Meteo->weather_code = json_integer_value(temp);
        temp = json_object_get(object, "timestamp");
        _Meteo->time_stamp = json_integer_value(temp);
        return 0;
    }
    else{
        printf("City does not exist!\n");
        return -1;
    }
    

    return 0;
}

int json_save_file(Meteo* _Meteo, const char* city_name)
{
    json_t* cities = NULL;

    cities = json_load_file("cities.json", 0, NULL);
    if(!cities){
        cities = json_array();
    }

    json_t* object = is_city_in_file(cities, city_name);

    if(_Meteo->needs_update || object == NULL){

        if(_Meteo->needs_update && object != NULL){
        printf("Needs update, fetching new data\n");
        json_remove_city(&cities, city_name);
        }
    
        if(object == NULL){
            printf("New city, adding to cache...\n");
        }
    json_t* city = json_object();
    json_object_set_new(city, "name", json_string(city_name));
    json_object_set_new(city, "temperature", json_real(_Meteo->temperature));
    json_object_set_new(city, "windspeed", json_real(_Meteo->wind_speed));
    json_object_set_new(city, "winddirection", json_integer(_Meteo->wind_direction));
    json_object_set_new(city, "is_day", json_boolean(_Meteo->is_day));
    json_object_set_new(city, "weathercode", json_integer(_Meteo->weather_code));
    json_object_set_new(city, "timestamp", json_integer(_Meteo->time_stamp));

    json_array_append_new(cities, city);

    if(json_dump_file(cities, "cities.json", JSON_INDENT(4)) != 0){
        printf("failed to write to file\n");
        json_decref(cities);
        return -1;
    }
    }
    else{
        printf("No need to update!\n");
        return -1;
    }

    return 0;
}

json_t* is_city_in_file(json_t* array, const char* city_name)
{
    size_t index;
    json_t* city;
    json_t* object;
    size_t size = json_array_size(array);

    for(index = 0; index < size; index++){
        object = json_array_get(array, index);
        city = json_object_get(object, "name");

        if(strcmp(json_string_value(city), city_name) == 0){
        return object;
        }
    }


    return NULL;
}

int json_remove_city(json_t** array, const char* city_name)
{

    if(*array == NULL)
    {
        return -1;
    }

    size_t index = 0;
    json_t* city;
    json_t* object;
    size_t size = json_array_size(*array);

    while(index < size){
        object = json_array_get(*array, index);
        city = json_object_get(object, "name");

        if(strcmp(json_string_value(city), city_name) == 0){
        json_array_remove(*array, index);
        size = json_array_size(*array);
        continue;
        }
        index++;
    }


    return 0;
}

bool json_needs_update(const char* city_name)
{
    json_t* cities = NULL;

    cities = json_load_file("cities.json", 0, NULL);
    if(!cities){
        cities = json_array();
    }

    json_t* object = is_city_in_file(cities, city_name);
    json_t* field = NULL;

    
    if (!object) {
        json_decref(cities);
        return true;
    }
    else{
        field = json_object_get(object, "timestamp");
    }

    time_t time_stamp = json_integer_value(field);
    time_t current = time(NULL);

    time_t difference = current - time_stamp;
    printf("current: %li timestamp: %li\n", current, time_stamp);
    if(difference > 600){
        json_decref(cities);
        return true;
    }
    json_decref(cities);
    return false;
}