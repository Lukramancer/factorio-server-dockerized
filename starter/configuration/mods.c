#include "../configuration.h"


#include "jq.h"
#include "../factorio.h"

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <spawn.h>


const char update_mods_list_from_env_jq_expression[] = {
#embed "update-mods-list-from-env.jq"
, 0
};

const char dlcs_configuration_json[] = {
#embed "dlcs.json"
, 0
};


int produce_mod_list_from_env(
    const char* base_mod_list_file_path,
    int destination_file_descriptor,
    int* error_ptr
) {
    int local_error_code;
    int* error_code_ptr = (error_ptr != NULL) ? error_ptr : &local_error_code;

    posix_spawn_file_actions_t jq_spawn_actions;
    *error_code_ptr = posix_spawn_file_actions_init(&jq_spawn_actions);
    if (*error_code_ptr != 0) return -1;
    
    *error_code_ptr = posix_spawn_file_actions_adddup2(&jq_spawn_actions,
        destination_file_descriptor, STDOUT_FILENO
    );
    if (*error_code_ptr != 0) {
        posix_spawn_file_actions_destroy(&jq_spawn_actions);
        return -1;
    }
    
    const char* jq_arguments[] = {
        "jq",
        "--argjson", "dlcs", dlcs_configuration_json,
        update_mods_list_from_env_jq_expression,
        base_mod_list_file_path,
        NULL
    };

    pid_t jq_process_id;
    *error_code_ptr = posix_spawn(
        &jq_process_id, JQ_BINARY_PATH, &jq_spawn_actions,
        NULL, (char *const*)jq_arguments, environ
    );
    if (*error_code_ptr != 0) {
        posix_spawn_file_actions_destroy(&jq_spawn_actions);
        return -1;
    }

    posix_spawn_file_actions_destroy(&jq_spawn_actions);
    
    
    int status;
    waitpid(jq_process_id, &status, 0);
    return WEXITSTATUS(status);
}


// Hacky way to generate mod-list.json via Factorio server executable and invalid scenario name
int generate_mod_list_file(
    const char* mods_directory_path,
    int* error_ptr
) {
    struct SaveCreationOptions save_creation_options;

    save_creation_options.mods_directory_path = mods_directory_path;
    
    save_creation_options.map_generation_settings_file_path = FACTORIO_MAP_GENERATION_EXAMPLE_SETTINGS_FILE_PATH;
    save_creation_options.preset = NULL;
    save_creation_options.map_generation_seed = NULL;

    struct StartOptions start_options = {
        .map_settings_file_path = FACTORIO_MAP_EXAMPLE_SETTINGS_FILE_PATH,
        .server_settings_file_path = FACTORIO_SERVER_EXAMPLE_SETTINGS_FILE_PATH,
        .server_adminlist_file_path = NULL,
        .server_banlist_file_path = NULL,
        .server_id_file_path = NULL,
        .server_whitelist_file_path = NULL,
        .use_server_whitelist = false,
        .use_authserver_bans = false,
        .mods_directory_path = NULL,
    };

    const char *start_scenario_arguments[30];

    place_start_scenario_arguments(start_scenario_arguments, "/", &start_options, &save_creation_options);

    int local_error_code;
    int* error_code_ptr = (error_ptr != NULL) ? error_ptr : &local_error_code;
    
    pid_t process_id;
    *error_code_ptr = posix_spawn(
        &process_id, FACTORIO_SERVER_EXECTUBALE_PATH, NULL,
        NULL, (char *const*)start_scenario_arguments, environ
    );
    if (*error_code_ptr != 0) return -1;

    
    int status;
    waitpid(process_id, &status, 0);
    return 0;
}