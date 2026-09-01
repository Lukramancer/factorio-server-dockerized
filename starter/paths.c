#include "paths.h"


#include "utils.h"


void init_paths_configuration(struct PathsConfiguration* paths_configuration_ptr) {
    paths_configuration_ptr->configs_directory_path = get_env_or_default("CONFIGS_DIRECTORY_PATH", FACTORIO_CONFIGS_DIRECTORY_PATH);
    paths_configuration_ptr->saves_directory_path = get_env_or_default("SAVES_DIRECTORY_PATH", FACTORIO_SAVES_DIRECTORY_PATH);
    paths_configuration_ptr->mods_directory_path = get_env_or_default("MODS_DIRECTORY_PATH", FACTORIO_MODS_DIRECTORY_PATH);

    paths_configuration_ptr->config_file_path = get_env_or_default("SERVER_CONFIG_FILE_PATH", FACTORIO_SERVER_CONFIG_FILE_PATH);
    
    // NOTE: might wanna make a _FILE_NAME options, which would specify a file name within configs directory
    paths_configuration_ptr->server_settings_file_path = get_env_or_default("SERVER_SETTINGS_FILE_PATH", FACTORIO_SERVER_SETTINGS_FILE_PATH);
    paths_configuration_ptr->server_whitelist_file_path = get_env_or_default("SERVER_WHITELIST_FILE_PATH", FACTORIO_SERVER_WHITELIST_FILE_PATH);
    paths_configuration_ptr->server_banlist_file_path = get_env_or_default("SERVER_BANLIST_FILE_PATH", FACTORIO_SERVER_BANLIST_FILE_PATH);
    paths_configuration_ptr->server_adminlist_file_path = get_env_or_default("SERVER_ADMINLIST_FILE_PATH", FACTORIO_SERVER_ADMINLIST_FILE_PATH);
    paths_configuration_ptr->server_id_file_path = get_env_or_default("SERVER_ID_FILE_PATH", FACTORIO_SERVER_ID_FILE_PATH);
    
    paths_configuration_ptr->map_settings_file_path = get_env_or_default("MAP_SETTINGS_FILE_PATH", FACTORIO_MAP_SETTINGS_FILE_PATH);
    paths_configuration_ptr->map_generation_settings_file_path = get_env_or_default("MAP_GENERATION_SETTINGS_FILE_PATH", FACTORIO_MAP_GENERATION_SETTINGS_FILE_PATH);
}