#include "../utils.h"

#include <string.h>


bool is_false(const char *string) {
    if (string == NULL) return true;
    
    if (strcmp(string, "false") == 0 || strcmp(string, "False") == 0 || strcmp(string, "FALSE") == 0) return true;

    if (strcmp(string, "no") == 0 || strcmp(string, "No") == 0 || strcmp(string, "NO") == 0) return true;

    for (const char *string_character_ptr = string; *string_character_ptr != '\0'; ++string_character_ptr) {
        if (*string_character_ptr != '0') return false;
    }
    return true;
}

bool is_true(const char *string) {
    return !is_false(string);
}