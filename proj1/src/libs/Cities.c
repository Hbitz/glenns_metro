#include "Cities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "HTTP.h"

//-------------Internal function definitions----------------

const char* Cities_list = "Stockholm:59.3293:18.0686\n" "Göteborg:57.7089:11.9746\n" "Malmö:55.6050:13.0038\n" "Uppsala:59.8586:17.6389\n" "Västerås:59.6099:16.5448\n" "Örebro:59.2741:15.2066\n" "Linköping:58.4109:15.6216\n" "Helsingborg:56.0465:12.6945\n" "Jönköping:57.7815:14.1562\n" "Norrköping:58.5877:16.1924\n" "Lund:55.7047:13.1910\n" "Gävle:60.6749:17.1413\n" "Sundsvall:62.3908:17.3069\n" "Umeå:63.8258:20.2630\n" "Luleå:65.5848:22.1567\n" "Kiruna:67.8558:20.2253\n";

void Cities_parse_list(Cities* _Cities, const char* list);

//..--------------------------------------------------------

int Cities_init(Cities* _Cities)
{
	memset(_Cities, 0, sizeof(Cities));
	
	Cities_parse_list(_Cities, Cities_list);

	Cities_print(_Cities);

	Meteo_init(&_Cities->meteo, "https://api.open-Meteo.com");

	return 0;
}

const char* Cities_get_list()
{
	return Cities_list;
}

void Cities_print(Cities* _Cities)
{
	City* current = _Cities->head;
	if(current == NULL)
	{
		printf("No Cities to print\n");
		return;
	}

	do
	{
		printf("City: %s, Latitude: %.4f, Longitude: %.4f\n", current->name, current->latitude, current->longitude);
		current = current->next;

	} while (current != NULL);

}

void Cities_parse_list(Cities* _Cities, const char* list)
{
	char* list_copy = strdup(list);
	if(list_copy == NULL)
	{
		printf("Failed to allocate memory for list copy\n");
		return;
	}

	char* ptr = list_copy;

	char* name = NULL;
	char* lat_str = NULL;
	char* lon_str = NULL;
	do
	{
		if(name == NULL)
		{
			name = ptr;
		}
		else if(lat_str == NULL)
		{
			if(*(ptr) == ':')
			{
				lat_str = ptr + 1;
				*(ptr) = '\0';
			}
		}
		else if(lon_str == NULL)
		{
			if(*(ptr) == ':')
			{
				lon_str = ptr + 1;
				*(ptr) = '\0';
			}
		}
		else
		{
			if(*(ptr) == '\n')
			{
				*(ptr) = '\0';

				//printf("City: <%s>, Latitude: <%s>, Longitude: <%s>\n", name, lat_str, lon_str);
				
				Cities_add(_Cities, name, atof(lat_str), atof(lon_str), NULL);

				name = NULL;
				lat_str = NULL;
				lon_str = NULL;
			}
		}

		ptr++;

	} while (*(ptr) != '\0');
	
	printf("Finished parsing City list\n");
}

int Cities_add(Cities* _Cities, char* _Name, float _Latitude, float _Longitude, City** _City)
{
	City* new_City = (City*)malloc(sizeof(City));
	if(new_City == NULL)
	{
		printf("Failed to allocate memory for new City\n");
		return -1;
	}

	new_City->name = _Name;
	new_City->latitude = _Latitude;
	new_City->longitude = _Longitude;
	
	new_City->prev = NULL;
	new_City->next = NULL;

	if(_Cities->tail == NULL)
	{
		_Cities->head = new_City;
		_Cities->tail = new_City;
	}
	else
	{
		new_City->prev = _Cities->tail;
		_Cities->tail->next = new_City;
		_Cities->tail = new_City;
	}
	
	if(_City != NULL)
		*(_City) = new_City;

	return 0;
}

int Cities_get(Cities* _Cities, const char* _Name, City** _CityPtr)
{
	City* current = _Cities->head;
	if(current == NULL)
		return -1;

	do
	{
		if(strcmp(current->name, _Name) == 0)
		{
			if(_CityPtr != NULL)
				*(_CityPtr) = current;
				
			return 0;
		}

		current = current->next;

	} while (current != NULL);
	
	return -1;
}

void Cities_remove(Cities* _Cities, City* _City)
{
	if(_City->next == NULL && _City->prev == NULL) 		//Jag är ensam
	{
		_Cities->head = NULL;
		_Cities->tail = NULL;
	}
	else if(_City->prev == NULL)						//Jag är först
	{
		_Cities->head = _City->next;
		_City->next->prev = NULL;
	}
	else if(_City->next == NULL)						//Jag är sist
	{
		_Cities->tail = _City->prev;
		_City->prev->next = NULL;
	}
	else												//Jag är i mitten
	{
		_City->prev->next = _City->next;
		_City->next->prev = _City->prev;
	}

	//Alla utfall är hanterade, frigör minnet

	free(_City);
}

int Cities_getTemperature(Cities* _Cities, City* _City)
{
	return Meteo_getTemperature(&_Cities->meteo, _City->latitude, _City->longitude, _City->name);
}

void Cities_dispose(Cities* c)
{
	c++;
}
