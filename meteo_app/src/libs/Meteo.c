#include "Meteo.h"
#include "HTTP.h"
#include "Json.h"
#include <string.h>
#include <stdio.h>

int Meteo_init(Meteo* meteo, const char* api_url)
{
	memset(meteo, 0, sizeof(Meteo));

	meteo->api_url = api_url;
	
	return 0;
}

int Meteo_setTimestamp(Meteo* meteo)
{
	meteo->time_stamp = time(NULL);
	return 0;
}



int Meteo_getTemperature(Meteo* meteo, float lat, float lon, const char* city_name)
{
	HTTP http;
	if(HTTP_init(&http) != 0)
	{
		printf("Failed to initialize HTTP\n");
		return -1;
	}

	char url[256];
	snprintf(url, sizeof(url), "%s/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true", meteo->api_url, lat, lon);
	
	meteo->needs_update = json_needs_update(city_name);

	if(meteo->needs_update)
	{
	int result = HTTP_get(&http, url);
	if(result != 0)
	{
		printf("HTTP GET request failed. Errorcode: %i\n", result);
		HTTP_dispose(&http);
		return -2;
	}
		json_parse_data(&http, meteo);
		Meteo_setTimestamp(meteo);
		json_save_file(meteo, city_name);
	}
	else
	{
		json_get_file(meteo, city_name);
	}

	
	printf("Temperature: %.2f °C\n", meteo->temperature);
	printf("Windspeed: %.2f \n", meteo->wind_speed);
	printf("Winddirection: %d \n", meteo->wind_direction);
	printf("Is Day: %d \n", meteo->is_day);
	printf("Weathercode: %d \n", meteo->weather_code);


	return 0;
}

void Meteo_dispose(Meteo* m)
{

}

