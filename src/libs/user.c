#include <stdio.h>
#include <string.h>
#include "Cities.h"

int get_user_input(char* buffer, size_t size) {
    if (!buffer || size == 0) {
        return 0;
    }
    
    if (fgets(buffer, size, stdin)) {
        // Remove newline character
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return 1;  // Success
    }
    return 0;  // Failure
}