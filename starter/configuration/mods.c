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
    const char* destination_file_path
) {
    int process_id = fork();
    if (process_id < 0) {
        return -1; // Could not fork process
    }
    else if (process_id == 0) {
        int destination_file_descriptor = open(destination_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (destination_file_descriptor < 0) {
            exit(-1); // Could not open destination file
        }
        
        int dup_return_code = dup2(destination_file_descriptor, STDOUT_FILENO);
        if (dup_return_code < 0) {
            exit(-1); // Could not duplicate fd
        }
        
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