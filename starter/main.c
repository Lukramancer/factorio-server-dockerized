#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#include "default_paths.h"
#include "options.h"
#include "factorio.h"
#include "utils.h"


int check_and_prepare_start_options(const struct StartOptions* start_options_ptr) {
    int server_settings_file_path_code = access(start_options_ptr->server_settings_file_path, F_OK);
    if (server_settings_file_path_code != 0 && errno == ENOENT) {
        int copy_example_file_code = copy_file(
            start_options_ptr->server_settings_file_path,
            FACTORIO_SERVER_EXAMPLE_SETTINGS_FILE_PATH
        );
        if (copy_example_file_code < 0) {
            return -1; // Could not copy example file
        }
    }
    else if (server_settings_file_path_code != 0) {
        return -1; // Could not access server settings file
    }

    return 0;
}

int check_and_prepare_save_creation_options(const struct SaveCreationOptions* start_options_ptr) {
    int map_generation_settings_file_path_code = access(start_options_ptr->map_generation_settings_file_path, F_OK);
    if (map_generation_settings_file_path_code != 0 && errno == ENOENT) {
        int copy_example_file_code = copy_file(
            start_options_ptr->map_generation_settings_file_path,
            FACTORIO_MAP_GENERATION_EXAMPLE_SETTINGS_FILE_PATH
        );
        if (copy_example_file_code < 0) {
            return -1; // Could not copy example file
        }
    }
    else if (map_generation_settings_file_path_code != 0) {
        return -1; // Could not access map generation settings file
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


void init_start(
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
) {
    char save_file_path[sizeof(FACTORIO_SAVES_DIRECTORY_PATH) + 1 + NAME_MAX];
    if (generate_save_file_path(FACTORIO_SAVES_DIRECTORY_PATH, save_file_path) != 0) {
        // TODO: report error
        return;
    }
    
    int save_creation_result = create_save_in_fork(
        save_file_path,
        save_creation_options_ptr
    );
    if (save_creation_result != 0) {
        // TODO: report error
        return;
    }
    
    start_save(save_file_path, start_options_ptr);
}


int gather_save_creation_options(struct SaveCreationOptions* destination_ptr) {
    destination_ptr->map_generation_seed = getenv("SEED");
    destination_ptr->preset = getenv("PRESET");
    
    destination_ptr->map_generation_settings_file_path = get_file_path_or_file_name_from_env(
        "MAP_GENERATION_SETTINGS_FILE_PATH",
        "MAP_GENERATION_SETTINGS_FILE_NAME",
        FACTORIO_MAP_GENERATION_SETTINGS_FILE_PATH
    );
    
    destination_ptr->map_settings_file_path = get_file_path_or_file_name_from_env(
        "MAP_SETTINGS_FILE_PATH",
        "MAP_SETTINGS_FILE_NAME",
        FACTORIO_MAP_SETTINGS_FILE_PATH
    );
    
    return 0;
}

int gather_start_options(struct StartOptions* destination_ptr) {
    destination_ptr->server_settings_file_path = get_file_path_or_file_name_from_env(
        "SERVER_SETTINGS_FILE_PATH",
        "SERVER_SETTINGS_FILE_NAME",
        FACTORIO_SERVER_SETTINGS_FILE_PATH
    );

    destination_ptr->server_id_file_path = get_file_path_or_file_name_from_env(
        "SERVER_ID_FILE_PATH",
        "SERVER_ID_FILE_NAME",
        FACTORIO_SERVER_ID_FILE_PATH
    );

    destination_ptr->use_server_whitelist = is_true(getenv("USE_SERVER_WHITELIST"));
    if (destination_ptr->use_server_whitelist) {
        destination_ptr->server_whitelist_file_path = get_file_path_or_file_name_from_env(
            "SERVER_WHITELIST_FILE_PATH",
            "SERVER_WHITELIST_FILE_NAME",
            FACTORIO_SERVER_WHITELIST_FILE_PATH
        );
    }

    destination_ptr->use_authserver_bans = is_true(getenv("USE_AUTHSERVER_BANS"));

    destination_ptr->server_banlist_file_path = get_file_path_or_file_name_from_env(
        "SERVER_BANLIST_FILE_PATH",
        "SERVER_BANLIST_FILE_NAME",
        FACTORIO_SERVER_BANLIST_FILE_PATH
    );

    if (is_true(getenv("USE_SERVER_ADMINLIST"))) {
        destination_ptr->server_adminlist_file_path = get_file_path_or_file_name_from_env(
            "SERVER_ADMINLIST_FILE_PATH",
            "SERVER_ADMINLIST_FILE_NAME",
            FACTORIO_SERVER_ADMINLIST_FILE_PATH
        );
    }
    
    destination_ptr->mods_directory_path = getenv("MODS_DIRECTORY_PATH");
    if (destination_ptr->mods_directory_path != NULL) destination_ptr->mods_directory_path = FACTORIO_MODS_DIRECTORY_PATH;

    return 0;
}


int main (int argc, const char *argv[]) {
    struct StartOptions start_options;
    struct SaveCreationOptions save_creation_options;

    gather_start_options(&start_options);
    
    const char *command = get_argv_or_environment_variable(argc, argv, 1, "COMMAND");
    
    if (command == NULL && has_save_file(FACTORIO_SAVES_DIRECTORY_PATH)) {
        start_latest(&start_options);
    }
    else if (command == NULL) {
        gather_save_creation_options(&save_creation_options);

        int code;
        if ((code = check_and_prepare_save_creation_options(&save_creation_options)) < 0) {
            printf("Could not prepare save creation options: code %i, errno %i", code, errno);
            return -1;
        }
        if ((code = check_and_prepare_start_options(&start_options)) < 0) {
            printf("Could not prepare start creation options: code %i, errno %i", code, errno);
            return -1;
        }
        
        init_start(&start_options, &save_creation_options);
    }
    else if (strcmp(command, "start-latest") == 0 || strcmp(command, "latest") == 0) {
        start_latest(&start_options);
    }
    else if (strcmp(command, "start-save") == 0 || strcmp(command, "save") == 0) {
        const char *save_file_name = get_argv_or_environment_variable(argc, argv, 2, "SAVE_FILE_NAME");

        if (save_file_name == NULL) {
            // TODO: report error
            return 1;
        }

        char save_file_path[sizeof(FACTORIO_SAVES_DIRECTORY_PATH) + 1 + NAME_MAX];
        strcpy(save_file_path, FACTORIO_SAVES_DIRECTORY_PATH);
        save_file_path[strlen(FACTORIO_SAVES_DIRECTORY_PATH)] = '/';
        strcpy(save_file_path + strlen(FACTORIO_SAVES_DIRECTORY_PATH) + 1, save_file_name);
        
        start_save(save_file_path, &start_options);
    }
    else if (strcmp(command, "start-scenario") == 0 || strcmp(command, "scenario") == 0) {
        const char *scenario = get_argv_or_environment_variable(argc, argv, 2, "SCENARIO");

        if (scenario == NULL) {
            // TODO: report error
            return 1;
        }

        gather_save_creation_options(&save_creation_options);
        
        start_scenario(scenario, &start_options, &save_creation_options);
    }
    else if (strcmp(command, "init-start") == 0 || strcmp(command, "init") == 0) {
        gather_save_creation_options(&save_creation_options);
        init_start(&start_options, &save_creation_options);
    }
}