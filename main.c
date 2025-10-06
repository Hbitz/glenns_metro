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
    City *city = NULL;
    int doStop = 0;
    while (doStop == 0)
    {
        city = NULL;
        Input_Command cmd = Input_SelectCity(cities, &city);
        switch (cmd)
        {
        case Input_Command_Error:
        {
            printf("An error occurred while selecting a city!\n");
        }
        break;

        case Input_Command_Exit:
        {
            doStop = 1;
        }
        break;

        case Input_Command_Invalid:
        {
            printf("Invalid input! Please try again.\n");
        }
        break;

        case Input_Command_OK:
        {
            int weatherMenuActive = 1;
            while (weatherMenuActive)
            {
                Weather_DisplayOptions();

                int choice;
                if (scanf("%d", &choice) != 1)
                {
                    printf("Invalid input! Please enter a number.\n");
                    while (getchar() != '\n')
                        ; // Clear input buffer
                    continue;
                }

                if (choice == 0)
                {
                    weatherMenuActive = 0; // Back to city selection
                    getchar(); // Consume newline
                    break;
                }
                json_t *weather_data = City_GetWeatherData(city);

                if (weather_data == NULL)
                {
                    printf("Failed to retrieve weather data for city %s!\n", city->name);
                    continue;
                }

                if (choice == 9)
                {
                    Weather_DisplayAllData(weather_data, city);
                    getchar(); // Consume newline
                    getchar();
                    weatherMenuActive = 0; // Back to city selection
                    json_decref(weather_data);
                    break;
                }

                if (choice < 1 || choice > 8)
                {
                    printf("Invalid choice! Please select 0-9.\n");
                    continue;
                }

                float value = 0.0f;
                char unit[16];
                char parameter_name[64];
                char* description = NULL;


                result =
                    Weather_GetParameter(weather_data, city, choice, &value, unit, parameter_name, &description);
                if (result != 0)
                {
                    printf("Failed to get %s for city %s! Errorcode: %i\n",
                           parameter_name, city->name, result);
                    continue;
                }

                printf("\n-----------------------------\n");
                printf("Current %s in %s: %.2f %s %s\n", parameter_name, city->name, value,
                       unit, description ? description : "");
                printf("-----------------------------\n\n");
                getchar(); // Consume newline
                getchar(); // Wait for user to press Enter
                json_decref(weather_data);
            }
        }
        break;

        default:
        {
            printf("An unknown error occurred while selecting a city!\n");
        }
        break;
        }
        
    }
    
    Cities_Dispose(&cities);
    return 0;
}
