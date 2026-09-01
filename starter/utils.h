#pragma once

#include <stdbool.h>
#include <stddef.h>


bool is_false(const char *string);

bool is_true(const char *string);


bool has_save_file(const char *directory_path);

int generate_save_file_path(const char* saves_directory_path, char* save_file_path);


bool does_directory_exist(const char* directory_path);


const char* get_env_or_default(const char* environment_variable_name, const char* default_value);

const char* get_argv_or_environment_variable(
    int argc, const char *argv[],
    size_t argv_number,
    const char* environment_variable_name
);

const char* get_argv_or_environment_variable_or_default(
    int argc, const char *argv[],
    size_t argv_number,
    const char* environment_variable_name,
    const char* default_value
);

int copy_file(const char* destination_path, const char* source_path);

const char* get_file_path_or_file_name_from_env(
    const char* file_path_env_variable_name,
    const char* file_name_env_variable_name,
    const char* default_value
);


void build_path(const char* directory_path, const char* file_name, char* destination);