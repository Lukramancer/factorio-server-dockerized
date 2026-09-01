#include "utils.h"

#define _GNU_SOURCE

#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>


bool does_directory_exist(const char* directory_path) {
    struct stat path_stats;
    if (stat(directory_path, &path_stats) != 0) {
        return false;
    }

    return S_ISDIR(path_stats.st_mode);
}


const char* get_env_or_default(
    const char* environment_variable_name,
    const char* default_value
) {
    const char* environment_variable_value;
    return ((environment_variable_value = getenv(environment_variable_name)) != NULL) ? environment_variable_value : default_value;
}


const char* get_argv_or_environment_variable(
    int argc, const char *argv[],
    size_t argv_number,
    const char* environment_variable_name
) {
    if (argc >= argv_number + 1 && argv[argv_number] != NULL) return argv[argv_number];

    return getenv(environment_variable_name);
}

const char* get_argv_or_environment_variable_or_default(
    int argc, const char *argv[],
    size_t argv_number,
    const char* environment_variable_name,
    const char* default_value
) {
    const char* result = get_argv_or_environment_variable(argc, argv, argv_number, environment_variable_name);
    if (result != NULL) return result;

    return default_value;
}


const char* get_file_path_or_file_name_from_env(
    const char* file_path_env_variable_name,
    const char* file_name_env_variable_name,
    const char* default_value
) {
    const char* result = getenv(file_path_env_variable_name);
    if (result != NULL) return result;

    result = getenv(file_name_env_variable_name);
    if (result != NULL) return result;

    return default_value;
}


void build_path(const char* directory_path, const char* file_name, char* destination) {
    strcpy(destination, directory_path);
    destination[strlen(directory_path)] = '/';
    strcpy(destination + strlen(directory_path) + 1, file_name);
}