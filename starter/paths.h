#pragma once


#ifndef FACTORIO_INSTALLATION_DIRECTORY_PATH
    #define FACTORIO_INSTALLATION_DIRECTORY_PATH "/opt/factorio"
#endif

#define FACTORIO_SERVER_EXECTUBALE_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/bin/x64/factorio"
#define FACTORIO_DATA_DIRECTORY_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/data"


#ifndef FACTORIO_SERVER_DATA_DIRECTORY_PATH
    #define FACTORIO_SERVER_DATA_DIRECTORY_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/server-data"
#endif

#ifndef FACTORIO_CONFIGS_DIRECTORY_PATH
    #define FACTORIO_CONFIGS_DIRECTORY_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/configs"
#endif

#define FACTORIO_SAVES_DIRECTORY_PATH FACTORIO_SERVER_DATA_DIRECTORY_PATH "/saves"

#ifndef FACTORIO_MODS_DIRECTORY_PATH
    #define FACTORIO_MODS_DIRECTORY_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/mods"
#endif


#define FACTORIO_SERVER_CONFIG_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/config.ini"
#define FACTORIO_SERVER_SETTINGS_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/server-settings.json"
#define FACTORIO_SERVER_WHITELIST_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/whitelist.json"
#define FACTORIO_SERVER_BANLIST_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/banlist.json"
#define FACTORIO_SERVER_ADMINLIST_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/adminlist.json"

#define FACTORIO_SERVER_ID_FILE_PATH FACTORIO_SERVER_DATA_DIRECTORY_PATH "/server-id.json"

#define FACTORIO_MAP_SETTINGS_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/map-settings.json"
#define FACTORIO_MAP_GENERATION_SETTINGS_FILE_PATH FACTORIO_CONFIGS_DIRECTORY_PATH "/map-gen-settings.json"

#define FACTORIO_SERVER_EXAMPLE_SETTINGS_FILE_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/data/server-settings.example.json"

#define FACTORIO_MAP_EXAMPLE_SETTINGS_FILE_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/data/map-settings.example.json"
#define FACTORIO_MAP_GENERATION_EXAMPLE_SETTINGS_FILE_PATH FACTORIO_INSTALLATION_DIRECTORY_PATH "/data/map-gen-settings.example.json"


struct PathsConfiguration {
    const char* config_file_path;
    
    const char* configs_directory_path;
    const char* saves_directory_path;
    const char* mods_directory_path;
    
    const char* server_settings_file_path;
    const char* server_whitelist_file_path;
    const char* server_banlist_file_path;
    const char* server_adminlist_file_path;
    const char* server_id_file_path;

    const char* map_settings_file_path;
    const char* map_generation_settings_file_path;
};

void init_paths_configuration(struct PathsConfiguration* paths_configuration_ptr);