#ifndef _CITY_H
#define _CITY_H

#include "jansson/jansson.h"

#define CITY_WEATHER_API_URL "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,precipitation,rain,showers,snowfall,weather_code,cloud_cover,pressure_msl,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m"

typedef struct City City;

typedef struct City
{
	char* name;
	float latitude;
	float longitude;

} City;


int City_Init(const char* _Name, const char* _Latitude, const char* _Longitude, City** _CityPtr);

int City_GetValue(City* _City, const char* _Name, float* _Value, char _Unit[16]);

void City_Dispose(City** _CityPtr);


#endif // _CITY_H