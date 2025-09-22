#include "Json.h"
#include <string.h>


/*Takes data stored in http->data, passes the data to json_str. Loads the data into root (jsont*)
Mostly a bunch of error checks, looks for the related fields and parses relevant data.*/


int json_parse_data(HTTP *http, Meteo *_Meteo)
{
    json_data data;

    data.json_str = http->data;
    data.root = json_loads(data.json_str, 0, &data.error);
    if (!data.root)
    {
        printf("failed parsing from buffer\n");
        fprintf(stderr, "JSON error: %s\n", data.error.text);
        free(data.json_str);
        return false;
    }
    data.current_weather = json_object_get(data.root, "current_weather");
    if (!data.current_weather)
    {
        printf("failed to find 'current_weather'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    data.object = json_object_get(data.current_weather, "temperature");
    if (!data.object)
    {
        printf("failed to find 'temperature'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->temperature = (float)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "windspeed");
    if (!data.object)
    {
        printf("failed to find 'windspeed'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->wind_speed = (float)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "winddirection");
    if (!data.object)
    {
        printf("failed to find 'winddirection'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->wind_direction = (int)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "is_day");
    if (!data.object)
    {
        printf("failed to find 'is_day'\n");
        free(data.json_str);
        json_decref(data.root);
        return false;
    }
    _Meteo->is_day = (bool)json_number_value(data.object);

    data.object = json_object_get(data.current_weather, "weathercode");
    if (!data.object)
    {
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


static json_t* cached_cities = NULL;


int json_get_file(Meteo *_Meteo, const char *city_name)
{
    json_t *cities = NULL;
    json_t *temp = NULL;
    // Load existing cities from file or create a new array if file doesn't exist
    cities = json_load_file("cities.json", 0, NULL);
    if (!cities)
    {
        cities = json_array();
    }
    // Find the city object in the array
    json_t *object = is_city_in_file(cities, city_name);

    if (object != NULL)
    {
        // City found, extract values
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
    }
    else
    {
        printf("City does not exist!\n");
        json_decref(cities);
        return -1;
    }

    json_decref(cities);
    return 0;
}

int json_save_file(Meteo *_Meteo, const char *city_name)
{
    json_t *cities = NULL;
    // Load existing cities from file or create a new array if file doesn't exist
    cities = json_load_file("cities.json", 0, NULL);
    if (!cities)
    {
        // File doesn't exist, create a new array
        cities = json_array();
    }


    json_t *object = is_city_in_file(cities, city_name);

    // Check if we need to update or if the city is new
    if (_Meteo->needs_update || object == NULL)
    {
        // If we need to update and the city exists, remove the old entry
        if (_Meteo->needs_update && object != NULL)
        {
            printf("Needs update, fetching new data\n");
            json_remove_city(&cities, city_name);
        }
        // If the city is new, just add it
        if (object == NULL)
        {
            printf("New city, adding to cache...\n");
        }
        
        json_t *city = json_object();
        json_object_set_new(city, "name", json_string(city_name));
        json_object_set_new(city, "temperature", json_real(_Meteo->temperature));
        json_object_set_new(city, "windspeed", json_real(_Meteo->wind_speed));
        json_object_set_new(city, "winddirection", json_integer(_Meteo->wind_direction));
        json_object_set_new(city, "is_day", json_boolean(_Meteo->is_day));
        json_object_set_new(city, "weathercode", json_integer(_Meteo->weather_code));
        json_object_set_new(city, "timestamp", json_integer(_Meteo->time_stamp));
        //Adds new city to array
        json_array_append_new(cities, city);

        if (json_dump_file(cities, "cities.json", JSON_INDENT(4)) != 0)
        {
            printf("failed to write to file\n");
            json_decref(cities);
            return -1;
        }
    }
    else
    {
        printf("No need to update!\n");

    }
    
    json_decref(cities);
    return 0;
}

json_t *is_city_in_file(json_t *array, const char *city_name)
{
    size_t index;
    json_t *city;
    json_t *object;
    size_t size = json_array_size(array);
    // Iterate through the array to find the city
    for (index = 0; index < size; index++)
    {
        // Get the city object at the current index
        object = json_array_get(array, index);
        // Get the "name" field from the city object
        city = json_object_get(object, "name");
        // Compare the city name with the provided city_name
        if (strcmp(json_string_value(city), city_name) == 0)
        {
            return object;
        }
    }

    return NULL;
}

int json_remove_city(json_t **array, const char *city_name)
{

    if (*array == NULL)
    {
        return -1;
    }

    size_t index = 0;
    json_t *city;
    json_t *object;
    size_t size = json_array_size(*array);

    while (index < size)
    {
        // Get the city object at the current index
        object = json_array_get(*array, index);
        // Get the "name" field from the city object
        city = json_object_get(object, "name");
        // Compare the city name with the provided city_name
        if (strcmp(json_string_value(city), city_name) == 0)
        {
            // Remove the city from the array
            json_array_remove(*array, index);
            // Update the size after removal
            size = json_array_size(*array);
            continue;
        }
        index++;
    }

    return 0;
}

bool json_needs_update(const char *city_name)
{
    json_t *cities = NULL;

    cities = json_load_file("cities.json", 0, NULL);
    if (!cities)
    {
        cities = json_array();
    }
    
    
    json_t *object = is_city_in_file(cities, city_name);
    json_t *field = NULL;

    if (!object)
    {
        json_decref(cities);
        return true;
    }
    else
    {
        // City exists, check the timestamp
        field = json_object_get(object, "timestamp");
    }

    // If the timestamp field is missing, we need to update
    time_t time_stamp = json_integer_value(field);
    time_t current = time(NULL);

    // Check if more than 60 minutes (3600 seconds) have passed
    time_t difference = current - time_stamp;
    printf("current: %li timestamp: %li\n", current, time_stamp);
    if (difference > 3600)
    {
        json_decref(cities);
        return true;
    }
    json_decref(cities);
    return false;
}