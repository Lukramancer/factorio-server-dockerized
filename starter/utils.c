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