#ifndef CITIES_H
#define CITIES_H

// Function declarations
void show_cities(void);
int find_city_coordinates(const char* city_name, double* lat, double* lon);
void generate_weather_url(double lat, double lon, char* url, size_t url_size);

#endif