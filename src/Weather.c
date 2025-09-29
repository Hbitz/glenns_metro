#include "Weather.h"
#include <stdio.h>
#include <string.h>

void Weather_DisplayOptions(void) {
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

int Weather_GetParameter(City* city, int choice, float* value, char* unit, char* parameter_name) {
    switch(choice) {
        case 1:
            strncpy(parameter_name, "Temperature", 63);
            return City_GetValue(city, "temperature_2m", value, unit);
        case 2:
            strncpy(parameter_name, "Wind Speed", 63);
            return City_GetValue(city, "wind_speed_10m", value, unit);
        case 3:
            strncpy(parameter_name, "Wind Direction", 63);
            return City_GetValue(city, "wind_direction_10m", value, unit);
        case 4:
            strncpy(parameter_name, "Weather Code", 63);
            return City_GetValue(city, "weather_code", value, unit);
        case 5:
            strncpy(parameter_name, "Humidity", 63);
            return City_GetValue(city, "relative_humidity_2m", value, unit);
        case 6:
            strncpy(parameter_name, "Pressure", 63);
            return City_GetValue(city, "pressure_msl", value, unit);
        case 7:
            strncpy(parameter_name, "Cloud Cover", 63);
            return City_GetValue(city, "cloud_cover", value, unit);
        case 8:
            strncpy(parameter_name, "Apparent Temperature", 63);
            return City_GetValue(city, "apparent_temperature", value, unit);
        default:
            return -1;
    }
}

void Weather_DisplayAllData(City* city) {
    printf("\n========== Complete Weather Data for %s ==========\n", city->name);
    
    float value;
    char unit[16];
    char param_name[64];
    
    for(int i = 1; i <= 8; i++) {
        if(Weather_GetParameter(city, i, &value, unit, param_name) == 0) {
            printf("%-20s: %.2f %s\n", param_name, value, unit);
        }
    }
    printf("================================================\n\n");
}