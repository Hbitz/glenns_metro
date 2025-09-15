#include <stdio.h>
#include <string.h>
#include "cities.h"

// Cities data
static const char* cities_data = 
    "Stockholm:59.3293:18.0686\n"
    "Göteborg:57.7089:11.9746\n"
    "Malmö:55.6050:13.0038\n"
    "Uppsala:59.8586:17.6389\n"
    "Västerås:59.6099:16.5448\n"
    "Örebro:59.2741:15.2066\n"
    "Linköping:58.4109:15.6216\n"
    "Helsingborg:56.0465:12.6945\n"
    "Jönköping:57.7815:14.1562\n"
    "Norrköping:58.5877:16.1924\n"
    "Lund:55.7047:13.1910\n"
    "Gävle:60.6749:17.1413\n"
    "Sundsvall:62.3908:17.3069\n"
    "Umeå:63.8258:20.2630\n"
    "Luleå:65.5848:22.1567\n"
    "Kiruna:67.8558:20.2253\n";

void show_cities(void) {
    char city[50];
    double lat, lon;
    const char* ptr = cities_data;
    
    while (sscanf(ptr, "%49[^:]:%lf:%lf", city, &lat, &lon) == 3) {
        printf("%s\n", city);
        ptr = strchr(ptr, '\n') + 1;
    }
}

int find_city_coordinates(const char* city_name, double* lat, double* lon) {
    char city[50];
    const char* ptr = strstr(cities_data, city_name);
    
    if (ptr && sscanf(ptr, "%49[^:]:%lf:%lf", city, lat, lon) == 3) {
        return 1; // Found
    }
    return 0; // Not found
}

void generate_weather_url(double lat, double lon, char* url, size_t url_size) {
    snprintf(url, url_size, 
        "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true", 
        lat, lon);
}