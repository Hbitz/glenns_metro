#ifndef CITIES_H
#define CITIES_H

#include "Meteo.h"


typedef struct City
{
	struct City* prev;
	struct City* next;

	char* name;
	float latitude;
	float longitude;

} City;

typedef struct
{
	City* head;
	City* tail;

	Meteo meteo;

} Cities;



int Cities_init(Cities* c);

void Cities_print(Cities* _Cities);
int Cities_add(Cities* _Cities, char* _Name, float _Latitude, float _Longitude, City** _City);
int Cities_get(Cities* _Cities, const char* _Name, City** _CityPtr);
void Cities_remove(Cities* _Cities, City* _City);
int Cities_getTemperature(Cities* _Cities, City* _City);
const char* Cities_get_list();


void Cities_dispose(Cities* c);

#endif // __CITIES_H__