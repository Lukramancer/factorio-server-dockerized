#pragma once

#include "options.h"


int create_save_in_fork(
    const char* save_file_path,
    const struct SaveCreationOptions* save_creation_options_ptr
);


void place_start_scenario_arguments(
    const char** arguments,
    const char* scenario,
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
);


void start_save(
    const char* save_file_path,
    const struct StartOptions* start_options_ptr
);

void start_latest(const struct StartOptions* start_options_ptr);

void start_scenario(
    const char* scenario,
    const struct StartOptions* start_options_ptr,
    const struct SaveCreationOptions* save_creation_options_ptr
);