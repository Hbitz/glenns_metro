#include "src/Cities.h"
#include <string.h>
#include "src/Input.h"
#include "src/Weather.h" // Add this include
#include <stdio.h>

int main()
{
    printf("\n--------------WeatherClient--------------\n"
           "Welcome to my awesome Weather Client!\n"
           "Type 'exit', 'quit' or 'q' to exit the program.\n\n");

    int result = 0;
    Cities *cities = NULL;

    result = Cities_Init(&cities);
    if (result != 0)
    {
        printf("Failed to initialize Cities struct! Errorcode: %i\n", result);
        return -1;
    }
    
    Weather_HandleInput(cities); // calling Weather_HandleMenu(City* city) if successful
    

    Cities_Dispose(&cities);
    return 0;
}
