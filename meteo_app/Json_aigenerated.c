#include "Json.h"
#include <string.h>

// Helper function to reduce repetitive JSON field extraction
static json_t* extract_field(json_t *parent, const char *field_name, const char *parent_name) {
    json_t *field = json_object_get(parent, field_name);
    if (!field) {
        printf("Failed to find '%s' in '%s'\n", field_name, parent_name);
    }
    return field;
}

int json_parse_data(HTTP *http, Meteo *_Meteo) {
    json_error_t error;
    json_t *root = json_loads(http->data, 0, &error);
    if (!root) {
        printf("Failed parsing JSON from buffer\n");
        fprintf(stderr, "JSON error: %s\n", error.text);
        return -1;
    }

    json_t *current_weather = extract_field(root, "current_weather", "root");
    if (!current_weather) {
        json_decref(root);
        return -1;
    }

    // Extract all fields at once with error checking
    json_t *fields[] = {
        extract_field(current_weather, "temperature", "current_weather"),
        extract_field(current_weather, "windspeed", "current_weather"),
        extract_field(current_weather, "winddirection", "current_weather"),
        extract_field(current_weather, "is_day", "current_weather"),
        extract_field(current_weather, "weathercode", "current_weather")
    };

    // Check if any field extraction failed
    for (int i = 0; i < 5; i++) {
        if (!fields[i]) {
            json_decref(root);
            return -1;
        }
    }

    // Assign values
    _Meteo->temperature = (float)json_number_value(fields[0]);
    _Meteo->wind_speed = (float)json_number_value(fields[1]);
    _Meteo->wind_direction = (int)json_number_value(fields[2]);
    _Meteo->is_day = (bool)json_number_value(fields[3]);
    _Meteo->weather_code = (int)json_number_value(fields[4]);

    json_decref(root);
    return 0;
}

// Cache the loaded cities array to avoid repeated file I/O
static json_t *cached_cities = NULL;
static time_t cache_load_time = 0;

static json_t* load_cities_cache() {
    time_t current_time = time(NULL);
    
    // Reload cache if it's older than 5 minutes or doesn't exist
    if (!cached_cities || (current_time - cache_load_time) > 300) {
        if (cached_cities) {
            json_decref(cached_cities);
        }
        
        cached_cities = json_load_file("cities.json", 0, NULL);
        if (!cached_cities) {
            cached_cities = json_array();
        }
        cache_load_time = current_time;
    }
    
    return cached_cities;
}

int json_get_file(Meteo *_Meteo, const char *city_name) {
    json_t *cities = load_cities_cache();
    json_t *city_object = is_city_in_file(cities, city_name);

    if (!city_object) {
        printf("City does not exist!\n");
        return -1;
    }

    printf("Found city in cache, getting values...\n");
    
    // Batch extract all fields
    struct {
        const char *name;
        json_t *field;
    } field_map[] = {
        {"temperature", NULL},
        {"windspeed", NULL},
        {"winddirection", NULL},
        {"is_day", NULL},
        {"weathercode", NULL},
        {"timestamp", NULL}
    };

    // Extract all fields
    for (int i = 0; i < 6; i++) {
        field_map[i].field = json_object_get(city_object, field_map[i].name);
        if (!field_map[i].field) {
            printf("Missing field: %s\n", field_map[i].name);
            return -1;
        }
    }

    // Assign values
    _Meteo->temperature = json_real_value(field_map[0].field);
    _Meteo->wind_speed = json_real_value(field_map[1].field);
    _Meteo->wind_direction = json_integer_value(field_map[2].field);
    _Meteo->is_day = json_boolean_value(field_map[3].field);
    _Meteo->weather_code = json_integer_value(field_map[4].field);
    _Meteo->time_stamp = json_integer_value(field_map[5].field);

    return 0;
}

int json_save_file(Meteo *_Meteo, const char *city_name) {
    json_t *cities = load_cities_cache();
    json_t *existing_city = is_city_in_file(cities, city_name);

    bool should_update = _Meteo->needs_update || (existing_city == NULL);
    
    if (!should_update) {
        printf("No need to update!\n");
        return -1;
    }

    // Remove existing city if updating
    if (_Meteo->needs_update && existing_city) {
        printf("Needs update, fetching new data\n");
        json_remove_city(&cities, city_name);
    } else if (!existing_city) {
        printf("New city, adding to cache...\n");
    }

    // Create new city object with all fields at once
    json_t *city = json_pack("{s:s, s:f, s:f, s:i, s:b, s:i, s:i}",
        "name", city_name,
        "temperature", _Meteo->temperature,
        "windspeed", _Meteo->wind_speed,
        "winddirection", _Meteo->wind_direction,
        "is_day", _Meteo->is_day,
        "weathercode", _Meteo->weather_code,
        "timestamp", _Meteo->time_stamp
    );

    if (!city) {
        printf("Failed to create city JSON object\n");
        return -1;
    }

    json_array_append_new(cities, city);

    if (json_dump_file(cities, "cities.json", JSON_INDENT(4)) != 0) {
        printf("Failed to write to file\n");
        return -1;
    }

    return 0;
}

json_t *is_city_in_file(json_t *array, const char *city_name) {
    if (!array || !city_name) return NULL;
    
    size_t array_size = json_array_size(array);
    
    for (size_t i = 0; i < array_size; i++) {
        json_t *city_object = json_array_get(array, i);
        json_t *name_field = json_object_get(city_object, "name");
        
        if (name_field && strcmp(json_string_value(name_field), city_name) == 0) {
            return city_object;
        }
    }
    
    return NULL;
}

int json_remove_city(json_t **array, const char *city_name) {
    if (!array || !*array || !city_name) return -1;

    // Remove in reverse order to avoid index shifting issues
    size_t array_size = json_array_size(*array);
    for (size_t i = array_size; i > 0; i--) {
        size_t index = i - 1;
        json_t *city_object = json_array_get(*array, index);
        json_t *name_field = json_object_get(city_object, "name");
        
        if (name_field && strcmp(json_string_value(name_field), city_name) == 0) {
            json_array_remove(*array, index);
        }
    }
    
    return 0;
}

bool json_needs_update(const char *city_name) {
    if (!city_name) return true;
    
    json_t *cities = load_cities_cache();
    json_t *city_object = is_city_in_file(cities, city_name);
    
    if (!city_object) return true;
    
    json_t *timestamp_field = json_object_get(city_object, "timestamp");
    if (!timestamp_field) return true;
    
    time_t stored_time = json_integer_value(timestamp_field);
    time_t current_time = time(NULL);
    time_t difference = current_time - stored_time;
    
    printf("Current: %ld, Stored: %ld, Difference: %ld\n", 
           current_time, stored_time, difference);
    
    return difference > 600; // 10 minutes
}

// Cleanup function to call before program exit
void json_cleanup() {
    if (cached_cities) {
        json_decref(cached_cities);
        cached_cities = NULL;
    }
}