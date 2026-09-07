#pragma once


#include "factorio.h"


int generate_default_config(const char* config_path);


int produce_server_settings_from_env(
    const char* base_server_settings_file_path,
    const char* destination_file_path,
    int* error
);


int produce_mod_list_from_env(
    const char* base_mod_list_file_path,
    int destination_file_descriptor,
    int* error_ptr
);

int generate_mod_list_file(
    const char* mods_directory_path
);