#include "options.h"


#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "arguments.h"
#include "utils.h"


void place_save_creation_options(const struct SaveCreationOptions* save_creation_options_ptr, const char*** options_ptr) {
    place_argument_with_value("--map-gen-settings", save_creation_options_ptr->map_generation_settings_file_path, options_ptr);
    place_optional_argument_with_value("--mod-directory", save_creation_options_ptr->mods_directory_path, options_ptr);
    place_optional_argument_with_value("--preset", save_creation_options_ptr->preset, options_ptr);
    place_optional_argument_with_value("--map-gen-seed", save_creation_options_ptr->map_generation_seed, options_ptr);
}


void place_start_options(const struct StartOptions* start_options_ptr, const char*** options_ptr) {
    place_argument_with_value("--server-settings", start_options_ptr->server_settings_file_path, options_ptr);
    place_argument_with_value("--map-settings", start_options_ptr->map_settings_file_path, options_ptr);
    if (start_options_ptr->use_server_whitelist) place_argument("--use-server-whitelist", options_ptr);
    if (start_options_ptr->use_authserver_bans) place_argument("--use-authserver-bans", options_ptr);
    place_optional_argument_with_value("--server-whitelist", start_options_ptr->server_whitelist_file_path, options_ptr);
    place_optional_argument_with_value("--server-banlist", start_options_ptr->server_banlist_file_path, options_ptr);
    place_optional_argument_with_value("--server-adminlist", start_options_ptr->server_adminlist_file_path, options_ptr);
    place_optional_argument_with_value("--server-id", start_options_ptr->server_id_file_path, options_ptr);
    place_optional_argument_with_value("--mod-directory", start_options_ptr->mods_directory_path, options_ptr);
}


void init_save_creation_options(
    struct SaveCreationOptions* destination_ptr,
    const struct PathsConfiguration* paths_configuration_ptr
) {
    destination_ptr->map_generation_seed = getenv("SEED");
    destination_ptr->preset = getenv("PRESET");
    
    destination_ptr->map_generation_settings_file_path = paths_configuration_ptr->map_generation_settings_file_path;

    destination_ptr->mods_directory_path = paths_configuration_ptr->mods_directory_path;
}


void init_start_options(
    struct StartOptions* destination_ptr,
    const struct PathsConfiguration* paths_configuration_ptr
) {
    destination_ptr->server_settings_file_path = paths_configuration_ptr->server_settings_file_path;
    destination_ptr->map_settings_file_path = paths_configuration_ptr->map_settings_file_path;
    destination_ptr->server_id_file_path = paths_configuration_ptr->server_id_file_path;

    destination_ptr->use_server_whitelist = is_true(getenv("USE_SERVER_WHITELIST"));
    if (destination_ptr->use_server_whitelist) {
        destination_ptr->server_whitelist_file_path = paths_configuration_ptr->server_whitelist_file_path;
    }
    else {
        destination_ptr->server_whitelist_file_path = NULL;
    }

    destination_ptr->use_authserver_bans = is_true(getenv("USE_AUTHSERVER_BANS"));

    destination_ptr->server_banlist_file_path = paths_configuration_ptr->server_banlist_file_path;

    if (is_true(getenv("USE_SERVER_ADMINLIST"))) {
        destination_ptr->server_adminlist_file_path = paths_configuration_ptr->server_adminlist_file_path;
    }
    else {
        destination_ptr->server_adminlist_file_path = NULL;
    }
    
    destination_ptr->mods_directory_path = paths_configuration_ptr->mods_directory_path;
}