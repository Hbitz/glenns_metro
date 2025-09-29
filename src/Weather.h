#ifndef WEATHER_H
#define WEATHER_H

#include "City.h"

// Weather parameter selection functions
void Weather_DisplayOptions(void);
int Weather_GetParameter(City* city, int choice, float* value, char* unit, char* parameter_name);
void Weather_DisplayAllData(City* city);

#endif