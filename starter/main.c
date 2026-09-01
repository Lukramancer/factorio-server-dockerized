#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#include "paths.h"
#include "options.h"
#include "factorio.h"
#include "configuration.h"
#include "utils.h"


int check_and_prepare_start_options(const struct StartOptions* start_options_ptr) {
    int server_settings_file_path_code = access(start_options_ptr->server_settings_file_path, F_OK);
    if (server_settings_file_path_code != 0 && errno == ENOENT) {
        int server_setting_infer_code = produce_server_settings_from_env(
            FACTORIO_SERVER_EXAMPLE_SETTINGS_FILE_PATH,
            start_options_ptr->server_settings_file_path
        );
        if (server_setting_infer_code < 0) {
            return -1; // Could not copy example file
        }
    }
    else if (server_settings_file_path_code != 0) {
        return -1; // Could not access server settings file
    }

    int map_settings_file_path_code = access(start_options_ptr->map_settings_file_path, F_OK);
    if (map_settings_file_path_code != 0 && errno == ENOENT) {
        int copy_example_file_code = copy_file(
            start_options_ptr->map_settings_file_path,
            FACTORIO_MAP_EXAMPLE_SETTINGS_FILE_PATH
        );
        if (copy_example_file_code < 0) {
            return -1; // Could not copy example file
        }
    }
    else if (map_settings_file_path_code != 0) {
        return -1; // Could not access map settings file
    }
    
    return 0;
}

int check_and_prepare_save_creation_options(const struct SaveCreationOptions* save_creating_options_ptr) {
    int map_generation_settings_file_path_code = access(save_creating_options_ptr->map_generation_settings_file_path, F_OK);
    if (map_generation_settings_file_path_code != 0 && errno == ENOENT) {
        int copy_example_file_code = copy_file(
            save_creating_options_ptr->map_generation_settings_file_path,
            FACTORIO_MAP_GENERATION_EXAMPLE_SETTINGS_FILE_PATH
        );
        if (copy_example_file_code < 0) {
            return -1; // Could not copy example file
        }
    }
    else if (map_generation_settings_file_path_code != 0) {
        return -1; // Could not access map generation settings file
    }
    
    return 0;
}


int init_run(
    struct PathsConfiguration* paths_configuration_ptr,
    struct StartOptions* start_options_ptr,
    int argc, const char *argv[]
) {
    if (!does_directory_exist(paths_configuration_ptr->saves_directory_path)) {
        mkdir(paths_configuration_ptr->saves_directory_path, 0755);
    }
    
    char save_file_path[PATH_MAX];
    
    int save_file_path_generation_result = generate_save_file_path(
        paths_configuration_ptr->saves_directory_path,
        save_file_path
    );
    if (save_file_path_generation_result != 0) {
        return -1; // Could not generate save file path
    }
    
    struct SaveCreationOptions save_creation_options;
    init_save_creation_options(&save_creation_options, paths_configuration_ptr);
    check_and_prepare_save_creation_options(&save_creation_options);
    
    int save_creation_result = create_save_in_fork(
        save_file_path,
        &save_creation_options
    );
    if (save_creation_result != 0) {
        return -1; // Could not create save
    }
    
    check_and_prepare_start_options(start_options_ptr);
    
    start_save(save_file_path, start_options_ptr);
    return 0;
}

int latest_run(
    struct PathsConfiguration* paths_configuration_ptr,
    struct StartOptions* start_options_ptr,
    int argc, const char *argv[]
) {
    check_and_prepare_start_options(start_options_ptr);
    
    start_latest(start_options_ptr);
    return 0;
}

int default_run(
    struct PathsConfiguration* paths_configuration_ptr,
    struct StartOptions* start_options_ptr,
    int argc, const char *argv[]
) {
    if (has_save_file(paths_configuration_ptr->saves_directory_path)) {
        return latest_run(paths_configuration_ptr, start_options_ptr, argc, argv);
    }
    
    check_and_prepare_start_options(start_options_ptr);
    
    return init_run(paths_configuration_ptr, start_options_ptr, argc, argv);
}

int save_run(
    struct PathsConfiguration* paths_configuration_ptr,
    struct StartOptions* start_options_ptr,
    int argc, const char *argv[]
) {
    const char *save_file_name = get_argv_or_environment_variable(argc, argv, 2, "SAVE_FILE_NAME");
    
    if (save_file_name == NULL) {
        return -1; // No save file name was specified
    }
    
    char save_file_path[PATH_MAX];
    build_path(paths_configuration_ptr->saves_directory_path, save_file_name, save_file_path);
    
    check_and_prepare_start_options(start_options_ptr);
    
    start_save(save_file_path, start_options_ptr);
    return 0;
}

int scenario_run(
    struct PathsConfiguration* paths_configuration_ptr,
    struct StartOptions* start_options_ptr,
    int argc, const char *argv[]
) {
    if (!does_directory_exist(paths_configuration_ptr->saves_directory_path)) {
        mkdir(paths_configuration_ptr->saves_directory_path, 0755);
    }
    
    const char *scenario = get_argv_or_environment_variable(argc, argv, 2, "SCENARIO");

    if (scenario == NULL) {
        return -1; // No scenario was specified
    }

    struct SaveCreationOptions save_creation_options;
    init_save_creation_options(&save_creation_options, paths_configuration_ptr);
    check_and_prepare_save_creation_options(&save_creation_options);

    check_and_prepare_start_options(start_options_ptr);
    
    start_scenario(scenario, start_options_ptr, &save_creation_options);
    return 0;
}


int main (int argc, const char *argv[]) {
    struct PathsConfiguration paths_configuration;
    init_paths_configuration(&paths_configuration);


    int config_file_access_code = access(paths_configuration.config_file_path, F_OK);
    if (config_file_access_code != 0 && errno == ENOENT) { 
        int config_file_generation_code = generate_default_config(paths_configuration.config_file_path);
        if (config_file_generation_code != 0) {
            return -1; // Could not generate config file, check errno
        }
    }
    else if (config_file_access_code != 0) {
        return -1; // Could not access config file, check errno
    }
    
    
    struct StartOptions start_options;
    init_start_options(&start_options, &paths_configuration);
    
    
    const char *command = get_argv_or_environment_variable(argc, argv, 1, "COMMAND");
    if (command == NULL) {
        default_run(&paths_configuration, &start_options, argc, argv);
    }
    else if (strcmp(command, "start-latest") == 0 || strcmp(command, "latest") == 0) {
        latest_run(&paths_configuration, &start_options, argc, argv);
    }
    else if (strcmp(command, "start-save") == 0 || strcmp(command, "save") == 0) {
        save_run(&paths_configuration, &start_options, argc, argv);
    }
    else if (strcmp(command, "start-scenario") == 0 || strcmp(command, "scenario") == 0) {
        scenario_run(&paths_configuration, &start_options, argc, argv);
    }
    else if (strcmp(command, "init-start") == 0 || strcmp(command, "init") == 0) {
        init_run(&paths_configuration, &start_options, argc, argv);
    }
}