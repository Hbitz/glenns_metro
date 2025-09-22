#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "libs/Cities.h"
#include "libs/user.h"

Cities g_cities;

// Helper function to trim whitespace
void trim_whitespace(char* str) {
    char* end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    // All spaces?
    if(*str == 0) return;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    end[1] = '\0';
}

int main()
{
    printf("=== Swedish Weather Application ===\n");
    printf("Initializing cities database...\n");
    
    if (Cities_init(&g_cities) != 0) {
        printf("Failed to initialize cities database\n");
        return 1;
    }

    char buffer[100];
    City* city = NULL;
    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts) {
        printf("\nEnter city name (or 'quit' to exit): ");
        
        if (!get_user_input(buffer, sizeof(buffer))) {
            printf("Error reading input\n");
            break;
        }
        
        // Trim whitespace
        trim_whitespace(buffer);
        
        if (strlen(buffer) == 0) {
            printf("Please enter a city name.\n");
            continue;
        }
        
        if (strcmp(buffer, "quit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("You entered: %s\n", buffer);
        
        if (Cities_get(&g_cities, buffer, &city) == 0) {
            printf("Found city: %s (%.4f, %.4f)\n", 
                   city->name, city->latitude, city->longitude);
            printf("Fetching weather data...\n");
            
            int result = Cities_getTemperature(&g_cities, city);
            if (result == 0) {
                printf("✓ Weather data retrieved successfully!\n");
            } else {
                printf("✗ Failed to get temperature, error code: %d\n", result);
                printf("This could be due to network issues or API problems.\n");
            }
            break;
        } else {
            attempts++;
            printf("✗ City '%s' not found.\n", buffer);
            
            if (attempts < max_attempts) {
                printf("Please check spelling. Available cities are listed above.\n");
                printf("(%d/%d attempts remaining)\n", max_attempts - attempts, max_attempts);
            } else {
                printf("Maximum attempts reached.\n");
            }
        }
    }

    printf("\nCleaning up...\n");
    Cities_dispose(&g_cities);
    printf("Application terminated.\n");
    
    return 0;
}