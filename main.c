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

    int doStop = 0;
    while (doStop == 0)
    {
        City *city = NULL;
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
                    break;
                }

                if (choice == 9)
                {
                    Weather_DisplayAllData(city);
                    getchar(); // Consume newline
                    getchar();
                    continue;
                }

                if (choice < 1 || choice > 8)
                {
                    printf("Invalid choice! Please select 0-9.\n");
                    continue;
                }

                float value = 0.0f;
                char unit[16];
                char parameter_name[64];

                result =
                    Weather_GetParameter(city, choice, &value, unit, parameter_name);
                if (result != 0)
                {
                    printf("Failed to get %s for city %s! Errorcode: %i\n",
                           parameter_name, city->name, result);
                    continue;
                }

                printf("\n-----------------------------\n");
                printf("Current %s in %s: %.2f %s\n", parameter_name, city->name, value,
                       unit);
                printf("-----------------------------\n\n");
                getchar(); // Consume newline
                getchar(); // Wait for user to press Enter
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

    return 0;
}
