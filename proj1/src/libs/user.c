#include <stdio.h>
#include <string.h>
#include "Cities.h"



void get_user_input(char* buffer, size_t size) {
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
    }
}