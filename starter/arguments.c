#include "arguments.h"


#include <unistd.h>


void place_argument(const char* option, const char*** destination) {
    (*((*destination)++)) = option;
}

void place_argument_with_value(const char* option_name, const char* option_value, const char*** destination) {
    place_argument(option_name, destination);
    place_argument(option_value, destination);
}

void place_optional_argument_with_value(const char* option_name, const char* option_value, const char*** destination) {
    if (option_value == NULL) return;
    
    return place_argument_with_value(option_name, option_value, destination);
}

void terminate_arguments(const char*** destination) {
    return place_argument(NULL, destination);
}