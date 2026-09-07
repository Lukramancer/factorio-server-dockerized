#include "factorio.h"


#define _GNU_SOURCE

#include <unistd.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <spawn.h>

#include "paths.h"
#include "arguments.h"
#include "options.h"


void place_create_save_arguments(
    const char** arguments,
    const char* save_file_path,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char **arguments_position_ptr = arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--create", save_file_path, &arguments_position_ptr);
    place_save_creation_options(save_creation_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
}

int create_save_in_fork(
    const char* save_file_path,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char *save_creation_arguments[10];
    place_create_save_arguments(
        save_creation_arguments,
        save_file_path,
        save_creation_options_ptr
    );
    
    int process_id;
    int posix_spawn_return_code = posix_spawn(
        &process_id,
        FACTORIO_SERVER_EXECTUBALE_PATH,
        NULL, NULL,
        (char* const*)save_creation_arguments,
        environ
    );
    if (posix_spawn_return_code != 0) return -1; // Could not posix_spawn

    int save_creation_child_process_exit_code;
    waitpid(process_id, &save_creation_child_process_exit_code, 0);

    return WEXITSTATUS(save_creation_child_process_exit_code);
}


void place_start_save_arguments(
    const char** arguments,
    const char* save_file_path,
    const struct StartOptions* start_options_ptr
) {
    const char **arguments_position_ptr = arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--start-server", save_file_path, &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
}

void start_save(
    const char* save_file_path,
    const struct StartOptions* start_options_ptr
) {
    const char *start_creation_arguments[30];
    place_start_save_arguments(
        start_creation_arguments,
        save_file_path,
        start_options_ptr
    );
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}


void place_start_latest_arguments(
    const char** arguments,
    const struct StartOptions* start_options_ptr
) {
    const char **arguments_position_ptr = arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument("--start-server-load-latest", &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
}

void start_latest(const struct StartOptions* start_options_ptr) {
    const char *start_creation_arguments[30];
    place_start_latest_arguments(start_creation_arguments, start_options_ptr);
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}


void place_start_scenario_arguments(
    const char** arguments,
    const char* scenario,
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char **arguments_position_ptr = arguments;
    place_argument("factorio", &arguments_position_ptr);
    place_argument_with_value("--start-server-load-scenario", scenario, &arguments_position_ptr);
    place_start_options(start_options_ptr, &arguments_position_ptr);
    place_save_creation_options(save_creation_options_ptr, &arguments_position_ptr);
    terminate_arguments(&arguments_position_ptr);
}

void start_scenario(
    const char* scenario,
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    const char *start_creation_arguments[30];

    place_start_scenario_arguments(
        start_creation_arguments,
        scenario,
        start_options_ptr, save_creation_options_ptr
    );
    
    execv(FACTORIO_SERVER_EXECTUBALE_PATH, (char* const*)start_creation_arguments);
}