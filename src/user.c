#include <stdio.h>
#include <string.h>
#include "cities.h"

void user_menu() 
{    
    printf("Welcome!\nAvailable cities:\n\n");
    show_cities();
        
    printf("\nEnter city name: ");
    
}

void get_user_input(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
    }
}