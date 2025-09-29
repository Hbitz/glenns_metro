#include <stdio.h>
#include <string.h>

#include "src/Cities.h"
#include "src/Input.h"

int main()
{
	printf(	"\n--------------WeatherClient--------------\n"
			"Welcome to my awesome Weather Client!\n"
			"Type 'exit', 'quit' or 'q' to exit the program.\n\n"
		);

	int result = 0;
	Cities* cities = NULL;

	result = Cities_Init(&cities);
	if(result != 0)
	{
		printf("Failed to initialize Cities struct! Errorcode: %i\n", result);
		return -1;
	}

	
	int doStop = 0;
	while(doStop == 0)
	{
		City* city = NULL;
		Input_Command cmd = Input_SelectCity(cities, &city);
		switch(cmd)
		{
			case Input_Command_Error:
			{
				printf("An error occurred while selecting a city!\n");
				
			} break;
			
			case Input_Command_Exit:
			{
				doStop = 1;				
			} break;

			case Input_Command_Invalid:
			{
				printf("Invalid input! Please try again.\n");
				
			} break;

			case Input_Command_OK:
			{
				float temperature = 0.0f;
				char unit[16];
				result = City_GetValue(city, "temperature_2m", &temperature, unit);
				if(result != 0)
				{
					printf("Failed to get temperature for city %s! Errorcode: %i\n", city->name, result);
					continue;
				}
				printf("\n-----------------------------\nCurrent temperature in %s: %.2f %s\n-----------------------------\n\n", city->name, temperature, unit);
				
			} break;

			default:
			{
				printf("An unknown error occurred while selecting a city!\n");

			} break;
		}

		
	}
	
	

	return 0;
}
