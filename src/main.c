#include <stdio.h>
#include <string.h>
#include "cities.h"

int main() {
    char input[50];
    double lat, lon;
    char url[200];
    
    printf("Welcome!\nAvailable cities:\n\n");
    
    while (1) {
        show_cities();
        
        printf("\nEnter city name: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        if (find_city_coordinates(input, &lat, &lon)) {
            generate_weather_url(lat, lon, url, sizeof(url));
            printf("Generated URL: %s\n", url);
        } else {
            printf("City not found.\n");
        }
        printf("\n");
    }
    
    return 0;

}