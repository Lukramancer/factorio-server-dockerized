#include "factorio.h"


#include <unistd.h>
#include <sys/wait.h>

#include "paths.h"
#include "arguments.h"
#include "options.h"


int create_save_in_fork(
    const char* save_file_path,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char *save_creation_arguments[10];

    const char **arguments_position_ptr = save_creation_arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--create", save_file_path, &arguments_position_ptr);
    place_save_creation_options(save_creation_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
    
    int process_id = fork();
    
    if (process_id == 0) {
        return execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)save_creation_arguments);
    }

    int save_creation_child_process_exit_code;
    waitpid(process_id, &save_creation_child_process_exit_code, 0);

    return save_creation_child_process_exit_code;
}


void start_save(
    const char* save_file_path,
    const struct StartOptions* start_options_ptr
) {
    const char *start_creation_arguments[30];

    const char **arguments_position_ptr = start_creation_arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--start-server", save_file_path, &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}

void start_latest(const struct StartOptions* start_options_ptr) {
    const char *start_creation_arguments[30];

    const char **arguments_position_ptr = start_creation_arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument("--start-server-load-latest", &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}

void start_scenario(
    const char* scenario,
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char *start_creation_arguments[30];

    const char **arguments_position_ptr = start_creation_arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--start-server-load-scenario", scenario, &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    place_save_creation_options(save_creation_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}