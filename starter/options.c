#include "options.h"


#include <stdbool.h>
#include <unistd.h>

#include "arguments.h"


void place_save_creation_options(const struct SaveCreationOptions* save_creation_options_ptr, const char*** options_ptr) {
    place_argument_with_value("--map-settings", save_creation_options_ptr->map_settings_file_path, options_ptr);
    place_argument_with_value("--map-gen-settings", save_creation_options_ptr->map_generation_settings_file_path, options_ptr);
    place_optional_argument_with_value("--preset", save_creation_options_ptr->preset, options_ptr);
    place_optional_argument_with_value("--map-gen-seed", save_creation_options_ptr->map_generation_seed, options_ptr);
}


void place_start_options(const struct StartOptions* start_options_ptr, const char*** options_ptr) {
    place_argument_with_value("--server-settings", start_options_ptr->server_settings_file_path, options_ptr);
    if (start_options_ptr->use_server_whitelist) place_argument("--use-server-whitelist", options_ptr);
    if (start_options_ptr->use_authserver_bans) place_argument("--use-authserver-bans", options_ptr);
    place_optional_argument_with_value("--server-whitelist", start_options_ptr->server_whitelist_file_path, options_ptr);
    place_optional_argument_with_value("--server-banlist", start_options_ptr->server_banlist_file_path, options_ptr);
    place_optional_argument_with_value("--server-adminlist", start_options_ptr->server_adminlist_file_path, options_ptr);
    place_argument_with_value("--server-id", start_options_ptr->server_id_file_path, options_ptr);
    place_optional_argument_with_value("--mod-directory", start_options_ptr->mods_directory_path, options_ptr);
}