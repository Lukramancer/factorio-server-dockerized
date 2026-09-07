#include "../configuration.h"


#define _GNU_SOURCE

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <spawn.h>

#include "jq.h"


const char update_server_setting_from_env_jq_expression[] = {
#embed "update-server-settings-from-env.jq"
, 0
};


int produce_server_settings_from_env(
    const char* base_server_settings_file_path,
    const char* destination_file_path,
    int* error_ptr
) {
    int local_error_code;
    int* error_code_ptr = (error_ptr != NULL) ? error_ptr : &local_error_code;

    posix_spawn_file_actions_t jq_spawn_actions;
    *error_code_ptr = posix_spawn_file_actions_init(&jq_spawn_actions);
    if (*error_code_ptr != 0) return -1;
    
    *error_code_ptr = posix_spawn_file_actions_addopen(&jq_spawn_actions,
        STDOUT_FILENO, destination_file_path,
        O_WRONLY | O_CREAT | O_TRUNC, 0644
    );
    if (*error_code_ptr != 0) {
        posix_spawn_file_actions_destroy(&jq_spawn_actions);
        return -1;
    }
    
    const char* jq_arguments[] = {
        "jq",
        update_server_setting_from_env_jq_expression,
        base_server_settings_file_path,
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