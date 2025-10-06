#ifndef WEATHER_H
#define WEATHER_H

#include "City.h"
#include "Input.h"

// Weather parameter selection functions
void Weather_DisplayOptions(void);
int Weather_GetParameter(json_t* weather_data, City* city, int choice, float* value, char* unit, char* parameter_name, char** description);
void Weather_DisplayAllData(json_t* weather_data, City* city);
void Weather_HandleMenu(City* city);
void City_InputCommand_Weathermenu(Cities* cities);

#endif