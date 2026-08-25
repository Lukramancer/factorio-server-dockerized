#pragma once

#include <stdbool.h>


struct SaveCreationOptions {
    const char* map_settings_file_path;
    const char* map_generation_settings_file_path;
    const char* preset;
    const char* map_generation_seed;
};

void place_save_creation_options(const struct SaveCreationOptions* save_creation_options_ptr, const char*** options_ptr);


struct StartOptions {
    const char* server_settings_file_path;
    bool use_authserver_bans;
    bool use_server_whitelist;
    const char* server_whitelist_file_path;
    const char* server_banlist_file_path;
    const char* server_adminlist_file_path;
    const char* server_id_file_path;
    const char* mods_directory_path;
};

void place_start_options(const struct StartOptions* start_options_ptr, const char*** options_ptr);