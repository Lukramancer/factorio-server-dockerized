#include "../configuration.h"


#include "jq.h"
#include "../factorio.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
    int destination_file_descriptor
) {
    int process_id = fork();
    if (process_id < 0) {
        return -1; // Could not fork process
    }
    else if (process_id == 0) {
        int dup_return_code = dup2(destination_file_descriptor, STDOUT_FILENO);
        if (dup_return_code < 0) {
            exit(-1); // Could not duplicate fd
        }

        close(destination_file_descriptor);
        
        int exec_return_code = execl(JQ_BINARY_PATH,
            "jq",
            "--argjson", "dlcs", dlcs_configuration_json,
            update_mods_list_from_env_jq_expression,
            base_mod_list_file_path,
            NULL
        );
        
        if (exec_return_code == -1) exit(-1); // Could not exec jq
        else exit(0);
        return 0;
    }

    int status;
    waitpid(process_id, &status, 0);
    return status;
}


// Hacky way to generate mod-list.json via Factorio server executable and invalid scenario name
int generate_mod_list_file(
    const char* mods_directory_path
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
    
    int process_id = fork();
    if (process_id < 0) {
        return -1; // Could not fork process
    }
    else if (process_id == 0) {
        start_scenario("/", &start_options, &save_creation_options);
        exit(0);
    }

    int status;
    waitpid(process_id, &status, 0);
    return 0;
}