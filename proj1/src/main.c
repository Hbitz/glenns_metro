#include <stdio.h>

#include "libs/Cities.h"
#include "libs/user.h"

Cities g_cities;

int main()
{


	Cities_init(&g_cities);

	City* _City;
	char buffer[100];
	get_user_input(buffer, 100);
	printf("You entered: %s\n", buffer);
	if(Cities_get(&g_cities, buffer, &_City) == 0)
	{
		float temperature; 
		int result = Cities_getTemperature(&g_cities, _City, &temperature);
		if(result == 0)
			printf("Temperature in Linköping: %.2f °C\n", temperature);
		else
			printf("Failed to get temperature for Linköping, error code: %d\n", result);


	}

	return 0;
}

