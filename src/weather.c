#include "user.h"
#include "weather.h"
#include <stdio.h>
#include "cities.h"


int run()
{
    char input[MAX_INPUT_LENGTH];
    double lat, lon;
    char url[MAX_URL_LENGTH];
    
    while (1) {        
        user_menu();
        get_user_input(input, sizeof(input));        
        if (find_city_coordinates(input, &lat, &lon)) {
            generate_weather_url(lat, lon, url, sizeof(url));
            printf("Generated URL: %s\n", url);
            http_get(url, NULL); // Placeholder for response handling
            
        } else {
            printf("City not found.\n");
        }
        printf("\n");
    }
    return 0;
}