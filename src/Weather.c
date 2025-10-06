#include "Weather.h"
#include <stdio.h>
#include <string.h>

void Weather_DisplayOptions() {
    printf("\n=== Weather Data Options ===\n");
    printf("1. Temperature (°C)\n");
    printf("2. Wind Speed (km/h)\n");
    printf("3. Wind Direction (°)\n");
    printf("4. Weather Code\n");
    printf("5. Humidity (%%)\n");
    printf("6. Pressure (hPa)\n");
    printf("7. Cloud Cover (%%)\n");
    printf("8. Apparent Temperature (°C)\n");
    printf("9. All Weather Data\n");
    printf("0. Back to city selection\n");
    printf("============================\n");
    printf("Choose option (0-9): ");
}


char* Weather_ConvertCode(int code) {
    switch(code) {
        case 0: return "Clear sky ☀️";
        case 1: return "Mainly clear 🌤️";
        case 2: return "Partly cloudy ⛅";
        case 3: return "Overcast ☁️";
        case 45: return "Fog 🌫️";
        case 48: return "Depositing rime fog 🌁";
        case 51: return "Light drizzle 🌦️";
        case 53: return "Moderate drizzle 🌧️";
        case 55: return "Dense drizzle 🌧️";
        case 56: return "Light freezing drizzle 🧊🌦️";
        case 57: return "Dense freezing drizzle ❄️🌧️";
        case 61: return "Slight rain 🌦️";
        case 63: return "Moderate rain 🌧️";
        case 65: return "Heavy rain ⛈️";
        case 66: return "Light freezing rain 🌧️🧊";
        case 67: return "Heavy freezing rain ❄️🌧️";
        case 71: return "Slight snow fall 🌨️";
        case 73: return "Moderate snow fall ❄️";
        case 75: return "Heavy snow fall 🌨️❄️";
        case 77: return "Snow grains 🌨️";
        case 80: return "Slight rain showers 🌦️";
        case 81: return "Moderate rain showers 🌧️";
        case 82: return "Violent rain showers ⛈️";
        case 85: return "Slight snow showers 🌨️";
        case 86: return "Heavy snow showers ❄️🌨️";
        case 95: return "Thunderstorm 🌩️";
        case 96: return "Thunderstorm with slight hail 🌩️🌨️";
        case 99: return "Thunderstorm with heavy hail ⛈️🌨️";
        default: return "Unknown weather code";
    }
}

int Weather_GetParameter(json_t* weather_data, City* city, int choice, float* value, char* unit, char* parameter_name, char** description) {
    switch(choice) {
        case 1:
            strcpy(parameter_name, "Temperature");
            return City_GetValue(weather_data, city, "temperature_2m", value, unit);
        case 2:
            strcpy(parameter_name, "Wind Speed");
            return City_GetValue(weather_data, city, "wind_speed_10m", value, unit);
        case 3:
            strcpy(parameter_name, "Wind Direction");
            return City_GetValue(weather_data, city, "wind_direction_10m", value, unit);
        case 4:
            strcpy(parameter_name, "Weather Code");
            int result = City_GetValue(weather_data, city, "weather_code", value, unit);
            *description = Weather_ConvertCode((int)(*value));
            return result;
        case 5:
            strcpy(parameter_name, "Humidity");
            return City_GetValue(weather_data, city, "relative_humidity_2m", value, unit);
        case 6:
            strcpy(parameter_name, "Pressure");
            return City_GetValue(weather_data, city, "pressure_msl", value, unit);
        case 7:
            strcpy(parameter_name, "Cloud Cover");
            return City_GetValue(weather_data, city, "cloud_cover", value, unit);
        case 8:
            strcpy(parameter_name, "Apparent Temperature");
            return City_GetValue(weather_data, city, "apparent_temperature", value, unit);
        default:
            return -1;
    }
}

void Weather_DisplayAllData(json_t* weather_data, City* city) {
    printf("\n========== Complete Weather Data for %s ==========\n", city->name);
    
    float value;
    char unit[16];
    char param_name[64];
    char* description;
    
    for(int i = 1; i <= 8; i++) {
        if(Weather_GetParameter(weather_data, city, i, &value, unit, param_name, &description) == 0) {
            if(i == 4) // Weather Code
                printf("%-20s: %s\n", param_name, description);
            else
                printf("%-20s: %.2f %s\n", param_name, value, unit);
        }
    }
    printf("================================================\n\n");
}

void Weather_HandleMenu(City* city) {
    int weatherMenuActive = 1;
    int result = 0;
    
    while (weatherMenuActive) {
        Weather_DisplayOptions();

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        if (choice == 0) {
            weatherMenuActive = 0; // Back to city selection
            getchar(); // Consume newline
            break;
        }
        
        json_t *weather_data = City_GetWeatherData(city);
        if (weather_data == NULL) {
            printf("Failed to retrieve weather data for city %s!\n", city->name);
            continue;
        }

        if (choice == 9) {
            Weather_DisplayAllData(weather_data, city);
            getchar(); // Consume newline
            getchar();
            weatherMenuActive = 0; // Back to city selection
            json_decref(weather_data);
            break;
        }

        if (choice < 1 || choice > 8) {
            printf("Invalid choice! Please select 0-9.\n");
            json_decref(weather_data);
            continue;
        }

        float value = 0.0f;
        char unit[16];
        char parameter_name[64];
        char* description = NULL;

        result = Weather_GetParameter(weather_data, city, choice, &value, unit, parameter_name, &description);
        
        if (result != 0) {
            printf("Failed to get %s for city %s! Errorcode: %i\n",
                   parameter_name, city->name, result);
            json_decref(weather_data);
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

void Weather_HandleInput(Cities* cities) {
    City *city = NULL;
    int doStop = 0;
    
    while (doStop == 0) {
        city = NULL;
        Input_Command cmd = Input_SelectCity(cities, &city);
        switch (cmd) {
        case Input_Command_Error:
            printf("An error occurred while selecting a city!\n");
            break;

        case Input_Command_Exit:
            doStop = 1;
            break;

        case Input_Command_Invalid:
            printf("Invalid input! Please try again.\n");
            break;

        case Input_Command_OK:
            Weather_HandleMenu(city);
            break;

        default:
            printf("An unknown error occurred while selecting a city!\n");
            break;
        }
    }
}