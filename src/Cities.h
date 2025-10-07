
#ifndef _CITIES_H
#define _CITIES_H
// #ifndef _GNU_SOURCE
// #define _GNU_SOURCE
// #endif

#include "utils/LinkedList.h"

typedef struct Cities Cities;

#include "City.h"

typedef struct Cities
{
	LinkedList list;
	
} Cities;

//-------------------------------

typedef struct {
	const char *key;
	const char *location;
	const char *location_unit;
} AllowedKey;

static const AllowedKey ALLOWED_KEYS[] = {
	{"temperature_2m", "current", "current_units"},
	{"latitude", NULL, NULL},
	{"longitude", NULL, NULL},
  };

static const size_t ALLOWED_KEYS_COUNT = sizeof(ALLOWED_KEYS) / sizeof(AllowedKey);
//-------------------------------


int Cities_Init(Cities** _CitiesPtr);

void Cities_WriteToFile(City _City);
void Cities_AddFromCache(Cities* _Cities);
void Cities_AddFromStringList(Cities* _Cities, const char* _StringList);
int Cities_Create(Cities* _Cities, const char* _Name, const char* _Latitude, const char* _Longitude, City** _City);
int Cities_GetName(Cities* _Cities, const char* _Name, City** _CityPtr);
int Cities_GetIndex(Cities* _Cities, int _Index, City** _CityPtr);
void Cities_Destroy(Cities* _Cities, City** _CityPtr);

void Cities_Print(Cities* _Cities);

void Cities_Dispose(Cities** _CitiesPtr);
char** Cities_Get_Names(Cities* _Cities, size_t* out_count);
LinkedList* Cities_GetCityValues(Cities* cities, const char *name);

#endif // _CITIES_H
