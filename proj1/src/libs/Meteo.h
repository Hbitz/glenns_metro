#ifndef __Meteo_h__
#define __Meteo_h__

#include <stdbool.h>
#include <time.h>

typedef struct
{
	const char* api_url;

	float temperature;
	float wind_speed;
	int wind_direction;
	int weather_code;
	bool is_day;
	bool needs_update;
	time_t time_stamp;

} Meteo;

int Meteo_init(Meteo* meteo, const char* api_url);

int Meteo_getTemperature(Meteo* meteo, float lat, float lon, const char* city_name);

void Meteo_dispose(Meteo* meteo);


#endif // __Meteo_h__
