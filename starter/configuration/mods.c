#include "../configuration.h"


#include "jq.h"
#include "../factorio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#ifndef KNOWN_FACTORIO_DLCS
    #define KNOWN_FACTORIO_DLCS "space-age"
#endif

const char* const factorio_dlcs[] = {KNOWN_FACTORIO_DLCS};
const size_t factorio_dlcs_amount = sizeof(factorio_dlcs) / sizeof(factorio_dlcs[0]);


#ifndef KNOWN_FACTORIO_FEATURES
    #define KNOWN_FACTORIO_FEATURES "elevated-rails", "quality"
#endif

const char* const factorio_features[] = {KNOWN_FACTORIO_FEATURES};
const size_t factorio_features_amount = sizeof(factorio_features) / sizeof(factorio_features[0]);


const char disable_mods_from_env_jq_expression[] = {
#embed "disable-mods-from-env.jq"
, 0
};


size_t count_json_string_length(const char* const elements[], size_t elements_amount) {
    // Empty list
    if (elements_amount == 0) return 2;
    
    size_t json_string_length = 0;
    for (size_t element_index = 0; element_index < elements_amount; ++element_index) {
        json_string_length += strlen(elements[element_index]);
    }
    // brackets + quotes + commas
    json_string_length += 2 + (elements_amount * 2) + (elements_amount - 1) * 1; 

    return json_string_length;
}

void create_json_list_string(
    const char* const elements[],
    size_t elements_amount,
    char* destination
) {
    char* current_position = destination;
    
    *(current_position++) = '[';

    for (size_t element_index = 0; (element_index + 1) < elements_amount; ++element_index) {
        current_position += sprintf(current_position, "\"%s\",", elements[element_index]);
    }

    if (elements_amount >= 1) sprintf(current_position, "\"%s\"]", elements[elements_amount - 1]);
    else sprintf(current_position, "]");
}


int produce_mod_list_from_env(
    const char* base_mod_list_file_path,
    const char* destination_file_path
) {
    int process_id = fork();
    if (process_id < 0) {
        return -1; // Could not fork process
    }
    else if (process_id == 0) {
        char* factorio_features_json_string = malloc((count_json_string_length(factorio_features, factorio_features_amount) + 1) * sizeof(char));
        if (factorio_features_json_string == NULL) {
            exit(-1); // Could not allocate memory
        }
        create_json_list_string(factorio_features, factorio_features_amount, factorio_features_json_string);
    
        char* factorio_dlcs_json_string = malloc((count_json_string_length(factorio_dlcs, factorio_dlcs_amount) + 1) * sizeof(char));
        if (factorio_features_json_string == NULL) {
            exit(-1); // Could not allocate memory
        }
        create_json_list_string(factorio_dlcs, factorio_dlcs_amount, factorio_dlcs_json_string);
        
        execl(JQ_BINARY_PATH,
            "jq",
            "--argjson", "known_dlcs", factorio_dlcs_json_string,
            "--argjson", "features", factorio_features_json_string,
            disable_mods_from_env_jq_expression,
            base_mod_list_file_path
        );
    
        free(factorio_features_json_string);
        free(factorio_dlcs_json_string);
    }

    return 0;
}


// Hacky way to generate mod-list.json via Factorio server executable and invalid scenario name
int generate_mod_list_file(
    const char* mods_directory_path,
    const struct StartOptions *start_options_ptr
) {
    struct SaveCreationOptions save_creation_options;

    save_creation_options.mods_directory_path = mods_directory_path;
    
    save_creation_options.map_generation_settings_file_path = FACTORIO_MAP_GENERATION_EXAMPLE_SETTINGS_FILE_PATH;
    save_creation_options.preset = NULL;
    save_creation_options.map_generation_seed = NULL;
    
    int process_id = fork();
    if (process_id < 0) {
        return -1; // Could not fork process
    }
    else if (process_id == 0) {
        start_scenario("/", start_options_ptr, &save_creation_options);
        exit(0);
    }

    int status;
    waitpid(process_id, &status, 0);
    return 0;
}