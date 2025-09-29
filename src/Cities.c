#include "Cities.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "utils/utils.h"
#include "tinydir.h"

const char* Cities_list = 	"Stockholm:59.3293:18.0686\n"
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

int Cities_Init(Cities** _CitiesPtr)
{
	if(_CitiesPtr == NULL)
		return -1;

	Cities* _Cities = (Cities*)malloc(sizeof(Cities));
	if(_Cities == NULL)
		return -2;

	LinkedList_Initialize(&_Cities->list);
	
	create_folder("cities");

	// load in from cities med tinydir, skapa ny lista till funktionen nedanför
	Cities_AddFromCache(_Cities);

	Cities_AddFromStringList(_Cities, Cities_list);

	*(_CitiesPtr) = _Cities;
	return 0;
}

void Cities_WriteToFile(City _City)
{
	json_t* root = json_object();
	if(root == NULL)
	{
		printf("Failed to create JSON root object for City %s\n", _City.name);
	}
	json_object_set_new(root, "name", json_string(_City.name));
	json_object_set_new(root, "latitude", json_real(_City.latitude));
	json_object_set_new(root, "longitude", json_real(_City.longitude));

	char filename[256];
	snprintf(filename, sizeof(filename), "cities/%s.json", _City.name);
	
	json_error_t error;
	json_t* filecheck = json_load_file(filename, 0, &error);
	if (!filecheck)
	{
		// file does not exist, create it
		int result = json_dump_file(root, filename, JSON_INDENT(4) | JSON_PRESERVE_ORDER);
		json_decref(root);
		if(result != 0)
		{
			printf("Failed to write City %s to file %s! Errorcode: %i\n", _City.name, filename, result);
		}
	}
	else
	{
		// file exists, do not overwrite
		json_decref(filecheck);
		json_decref(root);
		return;
	}
}

void Cities_AddFromCache(Cities* _Cities)
{
	if(_Cities == NULL)
		return;

	tinydir_dir dir;
	if (tinydir_open(&dir, "cities") == -1)
	{
		printf("Failed to open cities directory\n");
		return;
	}

	while (dir.has_next)
	{
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1)
		{
			printf("Failed to read file in cities directory\n");
			tinydir_close(&dir);
			return;
		}

		if (!file.is_dir)
		{
			// Only process .json files
			const char* ext = strrchr(file.name, '.');
			if (ext != NULL && strcmp(ext, ".json") == 0)
			{
				char filepath[256];
				snprintf(filepath, sizeof(filepath), "cities/%s", file.name);

				json_error_t error;
				json_t* root = json_load_file(filepath, 0, &error);
				if (!root)
				{
					printf("Failed to load JSON file %s: %s\n", filepath, error.text);
					dir.has_next ? tinydir_next(&dir) : 0;
					continue;
				}

				json_t* name_json = json_object_get(root, "name");
				json_t* lat_json = json_object_get(root, "latitude");
				json_t* lon_json = json_object_get(root, "longitude");

				if (!json_is_string(name_json) || !json_is_number(lat_json) || !json_is_number(lon_json))
				{
					printf("Invalid JSON format in file %s\n", filepath);
					json_decref(root);
					dir.has_next ? tinydir_next(&dir) : 0;
					continue;
				}

				const char* name = json_string_value(name_json);
				double latitude = json_number_value(lat_json);
				double longitude = json_number_value(lon_json);

				char lat_str[32];
				char lon_str[32];

				snprintf(lat_str, sizeof(lat_str), "%f", latitude);
				snprintf(lon_str, sizeof(lon_str), "%f", longitude);


				Cities_Create(_Cities, name, lat_str, lon_str, NULL);

				json_decref(root);
			}
		}

		dir.has_next ? tinydir_next(&dir) : 0;
	}

	tinydir_close(&dir);
}

void Cities_AddFromStringList(Cities* _Cities, const char* _StringList)
{
	char* list_copy = strdup(_StringList);
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

				int getName = Cities_GetName(_Cities, name, NULL);
				if (getName == 0) {
					printf("City already exists: %s\n", name);
				}
				else if (getName == -2) {
					Cities_Create(_Cities, name, lat_str, lon_str, NULL);
				}
				else {
					printf("Error checking city existence: %s\n", name);
				}

				name = NULL;
				lat_str = NULL;
				lon_str = NULL;
			}
		}

		ptr++;

	} while (*(ptr) != '\0');

	free(list_copy);
}

int Cities_Create(Cities* _Cities, const char* _Name, const char* _Latitude, const char* _Longitude, City** _City)
{
	if(_Cities == NULL || _Name == NULL)
		return -1;

	int result = 0;
	City* new_City = NULL;

	result = City_Init(_Name, _Latitude, _Longitude, &new_City);
	if(result != 0)
	{
		printf("Failed to initialize City struct! Errorcode: %i\n", result);
		return -2;
	}
	
	LinkedList_Push(&_Cities->list, new_City);
	// write_city_to_file if not exist
	Cities_WriteToFile(*new_City);


	
	if(_City != NULL)
		*(_City) = new_City;

	return 0;
}

int Cities_GetName(Cities* _Cities, const char* _Name, City** _CityPtr)
{
	if(_Cities == NULL || _Name == NULL)
		return -1;

	City* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		if(strcmp(city->name, _Name) == 0)
		{
			if(_CityPtr != NULL)
				*(_CityPtr) = city;

			return 0;
		}
	}
	
	return -2;
}

int Cities_GetIndex(Cities* _Cities, int _Index, City** _CityPtr)
{
	if(_Cities == NULL || _CityPtr == NULL)
		return -1;

	if(_Index < 0 || _Index >= _Cities->list.length)
		return -2;

	City* city = (City*)LinkedList_Get(&_Cities->list, _Index);
	if(city == NULL)
		return -3;

	*(_CityPtr) = city;

	return 0;
}

void Cities_Destroy(Cities* _Cities, City** _CityPtr)
{
	if(_Cities == NULL || _CityPtr == NULL || *(_CityPtr) == NULL)
		return;

	City* city = *(_CityPtr);
	LinkedList_Remove(&_Cities->list, city);
	City_Dispose(&city);

	*(_CityPtr) = NULL;
}

void Cities_Print(Cities* _Cities)
{
	if(_Cities == NULL)
		return;

	int index = 1;
	City* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		printf("[%i] - %s\n", index++, city->name);
	}
}

void Cities_Dispose(Cities** _CitiesPtr)
{
	if(_CitiesPtr == NULL || *(_CitiesPtr) == NULL)
		return;

	Cities* _Cities = *(_CitiesPtr);

	City* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		City_Dispose(&city);
	}

	LinkedList_Dispose(&_Cities->list);

	free(_Cities);

	*(_CitiesPtr) = NULL;
}

